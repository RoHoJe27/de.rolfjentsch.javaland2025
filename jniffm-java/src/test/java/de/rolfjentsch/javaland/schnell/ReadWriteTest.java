package de.rolfjentsch.javaland.schnell;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import de.rolfjentsch.javaland.Api;
import de.rolfjentsch.javaland.ApiInstrumentData;
import de.rolfjentsch.javaland.ApiStatus;
import de.rolfjentsch.javaland.ffm.FfmApi;
import de.rolfjentsch.javaland.ffm.implementation.api_h;
import de.rolfjentsch.javaland.jni.JniApi;

class ReadWriteTest
{
	private static Api ffmApi;
	private static Api jniApi;
	private final static ApiStatus NOT_FOUND = new ApiStatus(api_h.CB_STATUS_NODATA(), "Nicht vorhanden");
	private final static ApiStatus OK = new ApiStatus(api_h.CB_STATUS_OK(), "");

	@BeforeEach
	void setup()
	{
		FfmApi.reset();
		jniApi = new JniApi("JavaLand", "ZweiFünf");
		jniApi.init();
		ffmApi = new FfmApi("JavaLand", "ZweiFünf");
		ffmApi.init();
	}

	@Test
	void writeGetLs1lusTest()
	{ 	
		ApiInstrumentData data = new ApiInstrumentData("LS1LUS", "Lang & Schwarz AG", 1, 500_000.0);

		assertEquals(NOT_FOUND, jniApi.get(data.getInstrumentId()));
		assertEquals(NOT_FOUND, ffmApi.get(data.getInstrumentId()));

		assertEquals(OK, jniApi.insert(data));

		ApiStatus read = ffmApi.get(data.getInstrumentId());
		assertEquals(data, read.getInstrumentData());

		assertEquals(OK, jniApi.delete(data.getInstrumentId()));
		assertEquals(NOT_FOUND, ffmApi.delete(data.getInstrumentId()));
	}

	@Test
	void writeGetLx4p35Test()
	{
		ApiInstrumentData data = new ApiInstrumentData("LX4P35", "Endlos-Turbo-Zertifikat auf Gold / Put", 1,
				500_000.0);
		ApiInstrumentData update = new ApiInstrumentData("LX4P35", null, 5, null);
		ApiInstrumentData ende = new ApiInstrumentData("LX4P35", "Endlos-Turbo-Zertifikat auf Gold / Put", 5,
				500_000.0);

		assertEquals(OK, jniApi.insert(data));
		assertEquals(OK, jniApi.update(update));
		ApiStatus read = ffmApi.get(data.getInstrumentId());
		assertEquals(ende, read.getInstrumentData());

	}
}
