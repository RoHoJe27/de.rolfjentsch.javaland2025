package de.rolfjentsch.javaland;

/**
 * Interface das sowohl der JNI als auch der FFM-Wrapper implementieren.
 */
public interface Api
{
	/**
	 * Liefert die Version.
	 * 
	 * @return die Version als [major,minor,patch]
	 */
	int[] getVersion();

	/**
	 * Initialisiert die Schnittstelle.
	 * 
	 * @return der ApiStatus
	 */
	ApiStatus init();

	/**
	 * Fügt ein Instrument in die Datenhaltung ein.
	 * 
	 * @param data die Daten
	 * @return der ApiStatus
	 */
	ApiStatus insert(ApiInstrumentData data);

	/**
	 * aktualisiert ein Instrument in der Datenhaltung.
	 * 
	 * @param data die Daten
	 * @return der ApiStatus
	 */
	ApiStatus update(ApiInstrumentData data);

	/**
	 * Löscht ein Instrument aus der Datenhaltung.
	 * 
	 * @param instId die zu löschende ID
	 * @return der ApiStatus
	 */
	ApiStatus delete(String instId);

	/**
	 * Liest ein Instrument aus der Datenhaltung.
	 * 
	 * @param instId die zu lesende ID
	 * @return der ApiStatus. Enthält bei Erfolg ein ausgefüllte ApiInstrument
	 */
	ApiStatus get(String instId);
}
