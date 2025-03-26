package de.rolfjentsch.javaland;

import java.util.Objects;

/**
 * Kapselt die Daten für ein Instrument.
 * 
 * Nutzt wegen UPDATE keine primitiven Werte.
 */
public class ApiInstrumentData
{
	/** Die ID (WKN) */
	private String instrumentId;
	/** Der Name */
	private String name;
	/** Der Typ 1-5 */
	private Integer typ;
	/** Das maximale Handelsvolumen je Transaktion */
	private Double maxVolume;

	public ApiInstrumentData()
	{

	}

	public ApiInstrumentData(String instrumentId, String name, Integer typ, Double maxVolume)
	{
		this.instrumentId = instrumentId;
		this.name = name;
		this.typ = typ;
		this.maxVolume = maxVolume;
	}

	public String getInstrumentId()
	{
		return instrumentId;
	}

	public void setInstrumentId(String instrumentId)
	{
		this.instrumentId = instrumentId;
	}

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public Integer getTyp()
	{
		return typ;
	}

	public void setTyp(Integer typ)
	{
		this.typ = typ;
	}

	public Double getMaxVolume()
	{
		return maxVolume;
	}

	public void setMaxVolume(Double maxVolume)
	{
		this.maxVolume = maxVolume;
	}

	@Override
	public String toString()
	{
		return "ApiInstrumentData [instrumentId=" + instrumentId + ", name=" + name + ", typ=" + typ + ", maxVolume="
				+ maxVolume + "]";
	}

	@Override
	public int hashCode()
	{
		return Objects.hash(instrumentId, maxVolume, name, typ);
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
		ApiInstrumentData other = (ApiInstrumentData) obj;
		return Objects.equals(instrumentId, other.instrumentId) && Objects.equals(maxVolume, other.maxVolume)
				&& Objects.equals(name, other.name) && Objects.equals(typ, other.typ);
	}

}
