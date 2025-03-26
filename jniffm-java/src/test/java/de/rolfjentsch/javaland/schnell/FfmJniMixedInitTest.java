package de.rolfjentsch.javaland.schnell;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import de.rolfjentsch.javaland.Api;
import de.rolfjentsch.javaland.ApiStatus;
import de.rolfjentsch.javaland.ffm.FfmApi;
import de.rolfjentsch.javaland.jni.JniApi;

class FfmJniMixedInitTest
{
	@BeforeEach
	void setup()
	{
		FfmApi.reset();
	}

	@Test
	void jniFfmInitTest()
	{
		Api jniApi = new JniApi("JavaLand", "ZweiFünf");

		ApiStatus status = jniApi.init();
		assertEquals(0, status.getCode());

		Api ffmApi = new FfmApi("JavaLand", "ZweiFünf");

		status = ffmApi.init();
		assertEquals(0, status.getCode());
	}
}
