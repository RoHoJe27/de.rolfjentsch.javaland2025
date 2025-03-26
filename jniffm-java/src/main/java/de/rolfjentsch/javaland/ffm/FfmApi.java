package de.rolfjentsch.javaland.ffm;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Function;

import de.rolfjentsch.javaland.Api;
import de.rolfjentsch.javaland.ApiInstrumentData;
import de.rolfjentsch.javaland.ApiStatus;
import de.rolfjentsch.javaland.ffm.implementation.ApiStatusCb;
import de.rolfjentsch.javaland.ffm.implementation.InstrumentCb;
import de.rolfjentsch.javaland.ffm.implementation.InstrumentData;
import de.rolfjentsch.javaland.ffm.implementation.api_h;

/**
 * Die FFM-Implementierung für Api
 */
public class FfmApi implements Api
{
	/** Konstante für LATIN1 */
	private static final Charset LATIN1 = StandardCharsets.ISO_8859_1;
	/** Der Benutzername */
	private String user;
	/** Das Kennwort */
	private String password;
	/** Arena für die Callbacks. Lebt so lange wie das FfmApi-Objekt. */
	private Arena ffmApiAuto = Arena.ofAuto();
	/** Der ApiStatus für die Callbacks */
	private AtomicReference<ApiStatus> asyncStatus = new AtomicReference<>();
	/** Der Semaphor für die Synchronisation mit den Callbacks. */
	private Semaphore asyncSemaphore = new Semaphore(0);
	/** das apiHandle. */
	private int apiHandle = api_h.INVALID_API_HANDLE();

	/**
	 * Konstruktor
	 * 
	 * @param user     der Benutzername
	 * @param password das Kennwort
	 */
	public FfmApi(String user, String password)
	{
		this.user = user;
		this.password = password;
	}

	@Override
	public int[] getVersion()
	{
		MemorySegment memseg = Arena.ofAuto().allocate(api_h.C_INT, 3L);
		api_h.getApiVersion(memseg.asSlice(0L, Integer.BYTES), memseg.asSlice(Integer.BYTES, Integer.BYTES),
				memseg.asSlice(Integer.BYTES * 2, Integer.BYTES));
		return memseg.toArray(api_h.C_INT);
	}

	@Override
	public ApiStatus init()
	{
		Arena auto = Arena.ofAuto();
		apiHandle = api_h.createHandle(auto.allocateFrom(user, LATIN1), auto.allocateFrom(password, LATIN1));
		if (apiHandle > 0)
		{
			api_h.registerStatusCallback(apiHandle, ApiStatusCb.allocate(this::onStatusCallback, ffmApiAuto));
			api_h.registerInstrumentCallback(apiHandle,
					InstrumentCb.allocate(this::onInstrumentDataCallback, ffmApiAuto));
			return new ApiStatus(api_h.API_OK(), "OK");
		}
		else
		{
			int code = api_h.getLastErrorCode();
			return new ApiStatus(code, api_h.getErrorMessage(api_h.getLastErrorCode()).getString(0, LATIN1));
		}
	}

	@Override
	public ApiStatus insert(ApiInstrumentData data)
	{
		Arena auto = Arena.ofAuto();
		int result = api_h.instrumentCreate(apiHandle, auto.allocateFrom(data.getInstrumentId(), LATIN1),
				auto.allocateFrom(data.getName(), LATIN1), data.getTyp(), data.getMaxVolume());
		if (result == api_h.API_OK())
		{
			try
			{
				if (asyncSemaphore.tryAcquire(15, TimeUnit.SECONDS))
				{
					return asyncStatus.get();
				}
				else
				{
					return new ApiStatus(300, "Timeout");
				}
			}
			catch (InterruptedException e)
			{
				return new ApiStatus(301, "Internal");
			}
		}
		else
		{
			return new ApiStatus(result, api_h.getErrorMessage(result).getString(0, LATIN1));
		}
	}

	@Override
	public ApiStatus update(ApiInstrumentData data)
	{
		Arena auto = Arena.ofAuto();
		int result = api_h.instrumentUpdate(apiHandle, auto.allocateFrom(data.getInstrumentId(), LATIN1),
				data.getName() != null ? auto.allocateFrom(data.getName(), LATIN1) : auto.allocateFrom("", LATIN1),
				data.getName() != null ? 1 : 0, data.getTyp() != null ? data.getTyp() : 0,
				data.getTyp() != null ? 1 : 0, data.getMaxVolume() != null ? data.getMaxVolume() : 0.0,
				data.getMaxVolume() != null ? 1 : 0);
		if (result == api_h.API_OK())
		{
			try
			{
				if (asyncSemaphore.tryAcquire(15, TimeUnit.SECONDS))
				{
					return asyncStatus.get();
				}
				else
				{
					return new ApiStatus(300, "Timeout");
				}
			}
			catch (InterruptedException e)
			{
				return new ApiStatus(301, "Internal");
			}
		}
		else
		{
			return new ApiStatus(result, api_h.getErrorMessage(result).getString(0, LATIN1));
		}
	}

	@Override
	public ApiStatus delete(String instId)
	{
		Arena auto = Arena.ofAuto();
		int result = api_h.instrumentDelete(apiHandle, auto.allocateFrom(instId, LATIN1));
		if (result == api_h.API_OK())
		{
			try
			{
				if (asyncSemaphore.tryAcquire(15, TimeUnit.SECONDS))
				{
					return asyncStatus.get();
				}
				else
				{
					return new ApiStatus(300, "Timeout");
				}
			}
			catch (InterruptedException e)
			{
				return new ApiStatus(301, "Internal");
			}
		}
		else
		{
			return new ApiStatus(result, api_h.getErrorMessage(result).getString(0, LATIN1));
		}
	}

	@Override
	public ApiStatus get(String instId)
	{
		Arena auto = Arena.ofAuto();
		int result = api_h.instrumentRead(apiHandle, auto.allocateFrom(instId, LATIN1));
		if (result == api_h.API_OK())
		{
			try
			{
				if (asyncSemaphore.tryAcquire(15, TimeUnit.SECONDS))
				{
					return asyncStatus.get();
				}
				else
				{
					return new ApiStatus(300, "Timeout");
				}
			}
			catch (InterruptedException e)
			{
				return new ApiStatus(301, "Internal");
			}
		}
		else
		{
			return new ApiStatus(result, api_h.getErrorMessage(result).getString(0, LATIN1));
		}
	}

	/**
	 * Callback-Methode für den StatusCallback
	 * 
	 * @param handle           der Handle
	 * @param code             der StatusCode
	 * @param statusMsgSegment der StatusText
	 */
	private void onStatusCallback(int handle, int code, MemorySegment statusMsgSegment)
	{
		ApiStatus status = new ApiStatus(code, statusMsgSegment.getString(0, LATIN1));
		asyncStatus.set(status);
		asyncSemaphore.release();
	}

	/**
	 * Liest einen String für den InstrumentCallback
	 * 
	 * @param instrumentDataSegment das Datensegment
	 * @param getter                die Getter-Methode
	 * @return der String
	 */
	private String getString(MemorySegment instrumentDataSegment, Function<MemorySegment, MemorySegment> getter)
	{
		MemorySegment segment = getter.apply(instrumentDataSegment);
		if (segment.address() == 0L)
		{
			return null;
		}
		return segment.getString(0, LATIN1);
	}

	/**
	 * Callback für die InstrumentDaten (erfolgreiches get)
	 * 
	 * @param handle                der Handle
	 * @param instrumentDataSegment das Datensegment mit den Instrumentendaten.
	 */
	private void onInstrumentDataCallback(int handle, MemorySegment instrumentDataSegment)
	{
		ApiInstrumentData inst = new ApiInstrumentData();
		inst.setInstrumentId(getString(instrumentDataSegment, InstrumentData::instId));
		inst.setName(getString(instrumentDataSegment, InstrumentData::instName));
		inst.setTyp(InstrumentData.instType(instrumentDataSegment));
		inst.setMaxVolume(InstrumentData.maxVolume(instrumentDataSegment));
		// Hinweis: im Original kommen hier noch 31 weitere Zuweisungen.
		ApiStatus status = new ApiStatus(0, "OK", inst);
		asyncStatus.set(status);
		asyncSemaphore.release();
	}

	/**
	 * Setzt libapi auf den Anfangszustand zurück. Wird für die UnitTests benötigt.
	 */
	public static void reset()
	{
		api_h.reset();
	}
}
