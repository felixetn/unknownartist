package org.echtzeitsysteme.prosem.messages.bluetooth;

import org.echtzeitsysteme.prosem.Util;
import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.tuio.Point;

import TUIO.TuioObject;

public class GPSUpdateCoordMessage extends BluetoothMessage {
	private final static int BT_PORT = 11;

	private static int counter = 0;

	public GPSUpdateCoordMessage(boolean out, int channel, int destId,
			int srcId, int priority, int feldstaerke, TuioObject object,
			Point point, int camPort) {
		super(out, channel, destId, srcId, priority, feldstaerke, BT_PORT);
		setData(object, point, camPort);
	}

	public GPSUpdateCoordMessage(boolean out, int channel, int destId,
			int srcId, int priority, int feldstaerke, int[] payloadData) {
		super(out, channel, destId, srcId, priority, feldstaerke, BT_PORT);
		super.setData(payloadData);
	}

	public static Message parseRaw(int srcId, int feldstaerke, int destId,
			int priority, int[] data) {
		GPSUpdateCoordMessage msg = new GPSUpdateCoordMessage(false, 0, destId,
				srcId, priority, feldstaerke, data);
		return msg;
	};

	public void setData(TuioObject object, Point point, int camPort) {
		int[] gpsCoordPackage = new int[31];

		gpsCoordPackage[0] = counter & 0xff;
		gpsCoordPackage[1] = (counter >> 8) & 0xff;
		counter++;

		gpsCoordPackage[2] = camPort;
		gpsCoordPackage[3] = object.getSymbolID();

		int x = point.getXint();// normalize the float to short
		gpsCoordPackage[4] = x & 0xff;
		gpsCoordPackage[5] = (x >> 8) & 0xff;

		int y = point.getYint();// normalize the float to short
		gpsCoordPackage[6] = y & 0xff;
		gpsCoordPackage[7] = (y >> 8) & 0xff;

		int angle = point.getAngleInt(); // normalize the float to unsigned
											// short
		gpsCoordPackage[8] = angle & 0xff;
		gpsCoordPackage[9] = (angle >> 8) & 0xff;

		gpsCoordPackage[10] = 1; // mark as absolute coordinates

		byte[] xspeed = Util.getByteArrayOfFloat(object.getXSpeed());
		gpsCoordPackage[11] = xspeed[0] & 0xff;
		gpsCoordPackage[12] = xspeed[1] & 0xff;
		gpsCoordPackage[13] = xspeed[2] & 0xff;
		gpsCoordPackage[14] = xspeed[3] & 0xff;

		byte[] yspeed = Util.getByteArrayOfFloat(object.getYSpeed());
		gpsCoordPackage[15] = yspeed[0] & 0xff;
		gpsCoordPackage[16] = yspeed[1] & 0xff;
		gpsCoordPackage[17] = yspeed[2] & 0xff;
		gpsCoordPackage[18] = yspeed[3] & 0xff;

		byte[] rotspeed = Util.getByteArrayOfFloat(object.getRotationSpeed());
		gpsCoordPackage[19] = rotspeed[0] & 0xff;
		gpsCoordPackage[20] = rotspeed[1] & 0xff;
		gpsCoordPackage[21] = rotspeed[2] & 0xff;
		gpsCoordPackage[22] = rotspeed[3] & 0xff;

		byte[] motacc = Util.getByteArrayOfFloat(object.getMotionAccel());
		gpsCoordPackage[23] = motacc[0] & 0xff;
		gpsCoordPackage[24] = motacc[1] & 0xff;
		gpsCoordPackage[25] = motacc[2] & 0xff;
		gpsCoordPackage[26] = motacc[3] & 0xff;

		byte[] rotacc = Util.getByteArrayOfFloat(object.getRotationAccel());
		gpsCoordPackage[27] = rotacc[0] & 0xff;
		gpsCoordPackage[28] = rotacc[1] & 0xff;
		gpsCoordPackage[29] = rotacc[2] & 0xff;
		gpsCoordPackage[30] = rotacc[3] & 0xff;

		super.setData(gpsCoordPackage);
	}

	@Override
	public void printCommand() {
		super.printCommand();
		System.out.println("GPS UpdCoord Message:");
		System.out.println();
	}
}
