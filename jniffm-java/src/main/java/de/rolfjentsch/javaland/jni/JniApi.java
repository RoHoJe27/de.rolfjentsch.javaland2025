package de.rolfjentsch.javaland.jni;

import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;

import de.rolfjentsch.javaland.Api;
import de.rolfjentsch.javaland.ApiInstrumentData;
import de.rolfjentsch.javaland.ApiStatus;

/**
 * Die JNI-Implementierung für Api
 */
public class JniApi implements Api
{

	/**
	 * Lädt die Bibliothek mit dem JNI-Gluecode. Die Laufzeitumgebung muss so
	 * eingerichtet sein, dass das Betriebssystem darüber auch die shared-Library
	 * mit dem API laden kann.
	 */
	static
	{
		try
		{
			System.loadLibrary("jni");
		}
		catch (Exception e)
		{
			e.printStackTrace();
			System.exit(0);
		}
	}
	/** Benutzername (JavaLand) */
	private String user;
	/** Kennwort (ZweiFünf) */
	private String password;
	/** Benannter Name für das Encoding auf der C-Seite. */
	private static final Charset LATIN1 = StandardCharsets.ISO_8859_1;

	/**
	 * Konstruktor
	 * 
	 * @param user     der Benutzername
	 * @param password das Kennwort
	 */
	public JniApi(String user, String password)
	{
		this.user = user;
		this.password = password;
	}

	/**
	 * Das Handle für die weiteren Aufrufe.
	 */
	private int apiHandle = 0;

	@Override
	public ApiStatus init()
	{
		ApiStatus status = new ApiStatus();
		apiHandle = initHandle(user, password, status);
		return status;
	}

	/**
	 * Native-Deklaration für die initialsierung der JNI-Bibliothek.
	 * 
	 * @param user      der Benutzer
	 * @param password  das Kennwort
	 * @param apiStatus der API-Status
	 * @return das Handle
	 */
	private native int initHandle(String user, String password, ApiStatus apiStatus);

	@Override
	public ApiStatus insert(ApiInstrumentData data)
	{
		ApiStatus status = new ApiStatus();
		insert(apiHandle, data, status);
		return status;
	}

	/**
	 * Native Deklaration für den Insert
	 * 
	 * @param apiHandle das Handle
	 * @param data      die Daten
	 * @param status    der Status
	 * @return der direkte Status, ist auch in {@link ApiStatus#getCode()}
	 *         enthalten.
	 */
	private native int insert(int apiHandle, ApiInstrumentData data, ApiStatus status);

	@Override
	public ApiStatus update(ApiInstrumentData data)
	{
		ApiStatus status = new ApiStatus();
		update(apiHandle, data, status);
		return status;
	}

	/**
	 * Native Deklaration für den Update
	 * 
	 * @param apiHandle das Handle
	 * @param data      die Daten
	 * @param status    der Status
	 * @return der direkte Status, ist auch in {@link ApiStatus#getCode()}
	 *         enthalten.
	 */
	private native int update(int apiHandle, ApiInstrumentData data, ApiStatus status);

	@Override
	public ApiStatus delete(String id)
	{
		ApiStatus status = new ApiStatus();
		delete(apiHandle, id, status);
		return status;
	}

	/**
	 * Native Deklaration für das delete
	 * 
	 * @param apiHandle das Handle
	 * @param id        die ID
	 * @param status    der Status
	 * @return der direkte Status, ist auch in {@link ApiStatus#getCode()}
	 *         enthalten.
	 */
	private native int delete(int apiHandle, String id, ApiStatus status);

	@Override
	public ApiStatus get(String id)
	{
		ApiStatus status = new ApiStatus();
		get(apiHandle, id, status);
		return status;
	}

	/**
	 * Native Deklaration für das get
	 * 
	 * @param apiHandle das Handle
	 * @param id        die ID
	 * @param status    der Status
	 * @return der direkte Status, ist auch in {@link ApiStatus#getCode()}
	 *         enthalten.
	 */
	private native int get(int apiHandle, String id, ApiStatus status);

	@Override
	public int[] getVersion()
	{
		return getVersion(new int[3]);
	}

	/**
	 * Besorgt die Versionsinfo
	 * 
	 * @param versionBuffer Buffer für die Version
	 * @return der ausgefüllte Buffer
	 */
	private native int[] getVersion(int[] versionBuffer);

	/**
	 * Hilfsmethode um einen Java-String in ein LATIN1-kodiertes ByteArray
	 * umzuwandeln.
	 * 
	 * @param in der String
	 * @return das Bytearray
	 */
	byte[] getBytesLatin1(String in)
	{
		return in.getBytes(LATIN1);
	}

	/**
	 * Hilfsmethode um ein LATIN1-kodiertes ByteArray in einen Java-String
	 * umzuwandeln.
	 * 
	 * @param in das ByteArray
	 * @return der String
	 */
	String getStringLatin1(byte[] in)
	{
		return new String(in, LATIN1);
	}

}
