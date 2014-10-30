package org.echtzeitsysteme.prosem.messages.bluetooth;

import org.echtzeitsysteme.prosem.messages.Message;

public class GPSControlMessage extends BluetoothMessage {
	private final static int BT_PORT = 10;

	public enum EGPSControlMessageType {
		CONTROL_YOUR_ID(0),
		CONTROL_START(1),
		CONTROL_STOP(2),
		CONTROL_FOLLOW_THIS_ID(3),
		CONTROL_FOLLOW_LOWEST_ID(4),
		CONTROL_FOLLOW_OFFROAD(5),
		CONTROL_FOLLOW_OFFROAD_DYN(6),
		CONTROL_CARTOGRAPHY(7),
		CONTROL_SLAM(8);

		private int typeId;

		private EGPSControlMessageType(int id) {
			this.typeId = id;
		}

		public int getTypeId() {
			return typeId;
		}

		public static EGPSControlMessageType getType(int id) {
			for (EGPSControlMessageType t : EGPSControlMessageType.values())
				if (t.getTypeId() == id)
					return t;
			return null;
		}
	};

	private EGPSControlMessageType type;
	private int data;

	public GPSControlMessage(boolean out, int channel, int destId, int srcId,
			int priority, int feldstaerke, EGPSControlMessageType type, int data) {
		super(out, channel, destId, srcId, priority, feldstaerke, BT_PORT);
		setData(type, data);
	}

	public static Message parseRaw(int srcId, int feldstaerke, int destId,
			int priority, int[] data) {
		int type = data[0];
		int dataV = data[1];

		GPSControlMessage msg = new GPSControlMessage(false, 0, destId, srcId,
				priority, feldstaerke, EGPSControlMessageType.getType(type),
				dataV);
		return msg;
	};

	public void setData(EGPSControlMessageType type, int data) {
		this.type = type;
		this.data = data;
		int[] pkg = new int[2];
		pkg[0] = type.getTypeId();
		pkg[1] = data;
		super.setData(pkg);
	}

	@Override
	public void printCommand() {
		super.printCommand();
		System.out.println("GPS Ctrl Message:");
		System.out.println("  Type:   " + type);
		System.out.println("  Data:   " + data);
		System.out.println();
	}
}
