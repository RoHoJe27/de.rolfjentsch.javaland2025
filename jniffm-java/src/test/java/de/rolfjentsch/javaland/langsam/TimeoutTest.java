package de.rolfjentsch.javaland.langsam;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;

import de.rolfjentsch.javaland.Api;
import de.rolfjentsch.javaland.ApiStatus;
import de.rolfjentsch.javaland.ffm.FfmApi;
import de.rolfjentsch.javaland.jni.JniApi;

/**
 * In der libapi.so ist für die get-Funktion eine spezielle InstrumentID
 * implementiert: "CRASH!".
 * 
 * Die Abfrage dieser ID führt dazu, dass der Callback nicht aufgerufen wird und
 * ein Timeout-Ergeignis eintritt.
 */
class TimeoutTest
{
	private static final ApiStatus TIMEOUT = new ApiStatus(300, "Timeout");

	@Test
	void jniTimeoutTest()
	{
		Api jniApi = new JniApi("JavaLand", "ZweiFünf");
		ApiStatus status = jniApi.init();
		assertEquals(0, status.getCode());

		status = jniApi.get("CRASH!");
		assertEquals(TIMEOUT, status);
	}

	@Test
	void ffmTimeoutTest()
	{
		Api ffmApi = new FfmApi("JavaLand", "ZweiFünf");
		ApiStatus status = ffmApi.init();
		assertEquals(0, status.getCode());

		status = ffmApi.get("CRASH!");
		assertEquals(TIMEOUT, status);
	}

}
