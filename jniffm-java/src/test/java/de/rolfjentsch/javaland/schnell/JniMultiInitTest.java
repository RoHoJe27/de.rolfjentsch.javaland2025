package de.rolfjentsch.javaland.schnell;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.Arrays;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import de.rolfjentsch.javaland.Api;
import de.rolfjentsch.javaland.ApiStatus;
import de.rolfjentsch.javaland.ffm.FfmApi;
import de.rolfjentsch.javaland.jni.JniApi;

class JniMultiInitTest
{
	private static final int[] SOLL_VERSION =
	{ 2025, 4, 1 };

	@BeforeEach
	void setup()
	{
		FfmApi.reset();
	}

	@Test
	void jniInitMultiTest()
	{

		for (int i = 0; i < 10; i++)
		{
			Api jniApi = new JniApi("JavaLand", "ZweiFünf");

			ApiStatus status = jniApi.init();
			assertEquals(0, status.getCode());
			assertEquals("OK", status.getMessage());
			assertEquals(0, Arrays.compare(SOLL_VERSION, jniApi.getVersion()));
		}
		Api jniApi = new JniApi("JavaLand", "ZweiFünf");

		ApiStatus status = jniApi.init();
		assertEquals(3, status.getCode());
		assertEquals("maximum number of handles reached", status.getMessage());
		assertEquals(0, Arrays.compare(SOLL_VERSION, jniApi.getVersion()));

	}
}
