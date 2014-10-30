package org.echtzeitsysteme.prosem.messages.bluetooth;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Collection;
import java.util.LinkedHashMap;
import java.util.Vector;

import org.echtzeitsysteme.prosem.messages.HasSubTypes;
import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.messages.bluetooth.GPSControlMessage;
import org.echtzeitsysteme.prosem.messages.bluetooth.GPSUpdateCoordMessage;

public class BluetoothMessage extends Message implements HasSubTypes {
	private static LinkedHashMap<Integer, Class<? extends Message>> messageClassesByType = new LinkedHashMap<Integer, Class<? extends Message>>();
	static {
		messageClassesByType.put(10, GPSControlMessage.class);
		messageClassesByType.put(11, GPSUpdateCoordMessage.class);
	}

	private final static int MSG_TYPE = 11;
	
	private int btPort;
	private int[] btData;

	public BluetoothMessage(boolean out, int channel, int destId, int srcId, int priority, int feldstaerke, int btPort) {
		super(out, channel, destId, srcId, MSG_TYPE, priority, feldstaerke, new int[0]);
		this.btPort = btPort;
	}
	
	@Override
	protected void setData(int[] data) {
		this.btData = data;
		
		int real_len = data.length;

		for (int d : data)
			if (d == 0x03 || d == 0x02)
				real_len++;

		int[] packet = new int[real_len + 4];

		packet[0] = 0x02;
		packet[1] = 0x80 | (real_len & 0x7f);
		packet[2] = 0x80 | (btPort & 0x7f);

		int target = 3;
		for (int i = 0; i < data.length; i++) {
			if ((data[i] == 0x03) || (data[i] == 0x02)) { // BYTE STUFFING FOR
															// ETX
				packet[target] = data[i];
				target++;
			}
			packet[target] = data[i];
			target++;
		}
		packet[packet.length - 1] = 0x03;

		super.setData(packet);
	}

	public static Message parseRaw(int srcId, int feldstaerke, int destId, int priority, int[] data) {
		int btPort = data[2] & 0x7f;
		
		Vector<Integer> btDataDestuffed = new Vector<Integer>();
		for (int i = 3; i < data.length - 1; i++) {
			btDataDestuffed.add(data[i]);
			if ((data[i] == 0x02) || (data[i] == 0x03))
				i++;
		}
		
		int[] btPayload = new int[btDataDestuffed.size()];
		for (int i = 0; i < btPayload.length; i++)
			btPayload[i] = btDataDestuffed.get(i);
		
		if (messageClassesByType.containsKey(btPort)) {
			Class<? extends Message> c = messageClassesByType.get(btPort);
			try {
				Method m = c.getMethod("parseRaw", int.class, int.class, int.class, int.class, int[].class);
				Object o = m.invoke(null, srcId, feldstaerke, destId, priority, btPayload);
				if (o instanceof Message)
					return (Message) o;
			} catch (SecurityException e) {
				e.printStackTrace();
			} catch (NoSuchMethodException e) {
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				e.printStackTrace();
			}
		}
		BluetoothMessage btMsg = new BluetoothMessage(false, 0, destId, srcId, priority, feldstaerke, btPort);
		btMsg.setData(btPayload);
		return btMsg;
	};
	
	@Override
	public void printCommand() {
		super.printCommand();
		System.out.println("BT Message:");
		System.out.println("  Port:   " + btPort);
		System.out.print(  "  Data:   ");
		for (int i : btData) {
			System.out.print(String.format("%02x:", i));
		}
		System.out.println();
		System.out.println();
	}

	public static Collection<Class<? extends Message>> getMessageClasses() {
		Vector<Class<? extends Message>> cls = new Vector<Class<? extends Message>>();

		for (Class<? extends Message> c : messageClassesByType.values()) {
			cls.add(c);
			boolean hasSubTypes = false;
			for (Class<?> i : c.getInterfaces()) {
				if (i == HasSubTypes.class) {
					hasSubTypes = true;
					break;
				}
			}
			if (hasSubTypes) {
				try {
					Method m = c.getMethod("getMessageClasses");
					Object o = m.invoke(null);
					if (o instanceof Collection) {
						@SuppressWarnings("unchecked")
						Collection<Class<? extends Message>> subs = (Collection<Class<? extends Message>>) o;
						cls.addAll(subs);
					}
				} catch (SecurityException e) {
					e.printStackTrace();
				} catch (NoSuchMethodException e) {
					e.printStackTrace();
				} catch (IllegalArgumentException e) {
					e.printStackTrace();
				} catch (IllegalAccessException e) {
					e.printStackTrace();
				} catch (InvocationTargetException e) {
					e.printStackTrace();
				}
			}
		}

		return cls;
	}
}
