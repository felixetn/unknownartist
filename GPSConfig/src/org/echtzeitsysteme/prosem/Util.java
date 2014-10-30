package org.echtzeitsysteme.prosem;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class Util {
	public static int[] byteArrayToIntArray(byte[] ba) {
		int[] ia = new int[ba.length];
		for (int i = 0; i < ba.length; i++)
			ia[i] = ba[i] & 0xff;
		return ia;
	}
	
	public static byte[] getByteArrayOfFloat(float f) {
		ByteArrayOutputStream bos = new ByteArrayOutputStream(4);
		DataOutputStream dos = new DataOutputStream(bos);
		try {
			dos.writeFloat(f);
		} catch (IOException e) {
			e.printStackTrace();
		}
		byte[] fdata = bos.toByteArray();
		byte[] littleEndian = new byte[4];
		littleEndian[0] = fdata[3];
		littleEndian[1] = fdata[2];
		littleEndian[2] = fdata[1];
		littleEndian[3] = fdata[0];
		return littleEndian;
	}

}
