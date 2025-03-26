package de.rolfjentsch.javaland.schnell;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import de.rolfjentsch.javaland.Api;
import de.rolfjentsch.javaland.ApiStatus;
import de.rolfjentsch.javaland.ffm.FfmApi;

class FfmInitTest
{
	@BeforeEach
	void setup()
	{
		FfmApi.reset();
	}

	@Test
	void ffmInitTest()
	{
		Api ffmApi = new FfmApi("JavaLand", "ZweiFünf");

		ApiStatus status = ffmApi.init();
		assertEquals(0, status.getCode());
		assertEquals("OK", status.getMessage());
	}

	@Test
	void ffmInitFailTest()
	{
		Api ffmApi = new FfmApi("JavaLand", "ZweiVier");

		ApiStatus status = ffmApi.init();
		assertEquals(6, status.getCode());
		assertEquals("invalid user or password", status.getMessage());
	}
}
