package de.rolfjentsch.javaland.schnell;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import de.rolfjentsch.javaland.Api;
import de.rolfjentsch.javaland.ApiStatus;
import de.rolfjentsch.javaland.ffm.FfmApi;
import de.rolfjentsch.javaland.jni.JniApi;

class JniInitTest
{
	@BeforeEach
	void setup()
	{
		FfmApi.reset();
	}

	@Test
	void jniInitTest()
	{
		Api jniApi = new JniApi("JavaLand", "ZweiFünf");

		ApiStatus status = jniApi.init();
		assertEquals(0, status.getCode());
		assertEquals("OK", status.getMessage());
	}

	@Test
	void jniInitFailTest()
	{
		Api jniApi = new JniApi("JavaLand", "ZweiVier");

		ApiStatus status = jniApi.init();
		assertEquals(6, status.getCode());
		assertEquals("invalid user or password", status.getMessage());
	}
}
