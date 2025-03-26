package de.rolfjentsch.javaland.schnell;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import de.rolfjentsch.javaland.Api;
import de.rolfjentsch.javaland.ApiStatus;
import de.rolfjentsch.javaland.ffm.FfmApi;

class FfmMultiInitTest
{

	@BeforeEach
	void setup()
	{
		FfmApi.reset();
	}

	@Test
	void ffmInitMultiTest()
	{
		for (int i = 0; i < 10; i++)
		{
			Api ffmApi = new FfmApi("JavaLand", "ZweiFünf");

			ApiStatus status = ffmApi.init();
			assertEquals(0, status.getCode());
		}
		Api ffmApi = new FfmApi("JavaLand", "ZweiFünf");

		ApiStatus status = ffmApi.init();
		assertEquals(3, status.getCode());
	}
}
