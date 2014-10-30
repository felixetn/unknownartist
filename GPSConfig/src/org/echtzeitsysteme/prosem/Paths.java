package org.echtzeitsysteme.prosem;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;

public class Paths {

	/**
	 * Get the Jar path
	 * 
	 * @return Path
	 */
	public static File getProgramPath() {
		String jarPath = Paths.class.getProtectionDomain().getCodeSource()
				.getLocation().getPath();
		String decodedPath = null;
		try {
			decodedPath = URLDecoder.decode(jarPath, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		return new File(decodedPath).getParentFile();
	}

}
