package de.rolfjentsch.javaland;

import java.util.Objects;

/**
 * Kapselt das Ergebnis eines Aufrufs.
 */
public class ApiStatus
{
	/** Der Error oder Statuscode */
	private int code;
	/** Der Statustext. */
	private String message;
	/** Die Daten des Instruments, bei einer erfolgreichen Abfrage (GET) */
	private ApiInstrumentData instrumentData;

	/**
	 * Dieser Konstruktor wird für die JniApi benutzt.
	 */
	public ApiStatus()
	{

	}

	/**
	 * Dieser Konstruktor wird von FfmApi für den Statuscallback benötigt, sowie für
	 * Unittests.
	 * 
	 * @param code    der Statuscode
	 * @param message der Statuscode
	 */
	public ApiStatus(int code, String message)
	{
		this(code, message, null);
	}

	/**
	 * Dieser Konstruktor wird von FfmApi für den Instrumentdatacallback benötigt.
	 * 
	 * @param code           der Statuscode
	 * @param message        der Statuscode
	 * @param instrumentData die Instrumentdaten.
	 * 
	 */
	public ApiStatus(int code, String message, ApiInstrumentData instrumentData)
	{
		this.code = code;
		this.message = message;
		this.instrumentData = instrumentData;
	}

	public int getCode()
	{
		return code;
	}

	public void setCode(int code)
	{
		this.code = code;
	}

	public String getMessage()
	{
		return message;
	}

	public void setMessage(String message)
	{
		this.message = message;
	}

	public ApiInstrumentData getInstrumentData()
	{
		return instrumentData;
	}

	public void setInstrumentData(ApiInstrumentData instrumentData)
	{
		this.instrumentData = instrumentData;
	}

	@Override
	public String toString()
	{
		return "ApiStatus [code=" + code + ", message=" + message + ", instrumentData=" + instrumentData + "]";
	}

	@Override
	public int hashCode()
	{
		return Objects.hash(code, instrumentData, message);
	}

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		ApiStatus other = (ApiStatus) obj;
		return code == other.code && Objects.equals(instrumentData, other.instrumentData)
				&& Objects.equals(message, other.message);
	}

}
