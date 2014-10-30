package org.echtzeitsysteme.prosem.messages;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Collection;
import java.util.LinkedHashMap;
import java.util.Vector;

import org.echtzeitsysteme.prosem.messages.bluetooth.BluetoothMessage;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands.CMD_DATAEX_REQ;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands.CMD_DATA_REPLY;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands.Command;

public class Message extends CMD_DATAEX_REQ implements HasSubTypes {
	private static LinkedHashMap<Integer, Class<? extends Message>> messageClassesByType = new LinkedHashMap<Integer, Class<? extends Message>>();
	static {
		messageClassesByType.put(0, DebugMessage.class);
		messageClassesByType.put(0x0b, BluetoothMessage.class);
	}

	public enum EMessageState {
		RECV,
		NOT_SENT,
		TIMEOUT,
		ACK,
		NO_ACK
	};

	private int destinationId;
	private int sourceId;

	private int messageType;
	private int priority;
	private int[] msgDataInt;

	private int feldstaerke;

	private String dataAsHex;
	private String dataAsASCII;

	private EMessageState msgState = EMessageState.RECV;

	// To Manually create a Message
	public Message(boolean out, int channel, int destinationId, int sourceId,
			int messageType, int priority, int feldstaerke, int[] data) {
		super(out, channel, destinationId);
		this.destinationId = destinationId;
		this.sourceId = sourceId;
		this.messageType = messageType;
		this.priority = priority;
		this.feldstaerke = feldstaerke;
		if (out)
			msgState = EMessageState.NOT_SENT;
		setData(data);
	}

	public static Message parseRaw(int srcId, int feldstaerke, int[] data) {
		int destId = data[0];
		int messageType = data[1];
		int priority = data[2];
		int[] payload = new int[data.length - 3];
		System.arraycopy(data, 3, payload, 0, payload.length);
		
		if (messageClassesByType.containsKey(messageType)) {
			Class<? extends Message> c = messageClassesByType.get(messageType);
			try {
				Method m = c.getMethod("parseRaw", int.class, int.class, int.class, int.class, int[].class);
				Object o = m.invoke(null, srcId, feldstaerke, destId, priority, payload);
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
		return new Message(false, 0, destId, srcId, messageType, priority,
				feldstaerke, payload);
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

	private void updateDataRepresentations() {
		StringBuffer dataHex = new StringBuffer(msgDataInt.length * 3);
		StringBuffer dataASCII = new StringBuffer(msgDataInt.length);
		for (int d : msgDataInt) {
			if (dataHex.length() > 0)
				dataHex.append(":");
			dataHex.append(String.format("%02x", d));
			dataASCII.append((char) d);
		}
		dataAsHex = dataHex.toString();
		dataAsASCII = dataASCII.toString();
	}

	// ##################### Getter and Setter ######################

	@Override
	public void printCommand() {
		System.out.println();
		System.out.println("Message:");
		System.out.println("  Source: " + sourceId);
		System.out.println("  Dest:   " + destinationId);
		System.out.println("  Prio:   " + priority);
		System.out.println("  Type:   " + messageType);
		System.out.println("  Ack:    " + msgState);
		System.out.print("  Data:   ");
		for (int i : msgDataInt) {
			System.out.print(String.format("%02x:", i));
		}
		System.out.println();
		System.out.println();
	}

	public int getDestinationId() {
		return destinationId;
	}

	public int[] getData() {
		return msgDataInt;
	}

	public int getMessageType() {
		return messageType;
	}

	public int getPriority() {
		return priority;
	}

	public int getSourceId() {
		return sourceId;
	}

	public int getFeldstaerke() {
		return feldstaerke;
	}

	public EMessageState getMessageState() {
		return msgState;
	}

	public String getDataAsASCII() {
		return dataAsASCII;
	}

	public String getDataAsHex() {
		return dataAsHex;
	}

	@Override
	public void setReply(Command reply) {
		super.setReply(reply);
		if (reply == null) {
			msgState = EMessageState.TIMEOUT;
		} else if (reply instanceof CMD_DATA_REPLY) {
			if (((CMD_DATA_REPLY) reply).hasAck())
				msgState = EMessageState.ACK;
			else
				msgState = EMessageState.NO_ACK;
		}
	}

	protected void setData(int[] data) {
		msgDataInt = new int[data.length];
		for (int i = 0; i < data.length; i++) {
			msgDataInt[i] = data[i] & 0xff;
		}
		updateDataRepresentations();

		int[] packed = new int[msgDataInt.length + 3];
		packed[0] = (byte) destinationId; // Dest ID in package payload
		packed[1] = (byte) messageType; // Message type
		packed[2] = (byte) priority; // Priority
		System.arraycopy(msgDataInt, 0, packed, 3, msgDataInt.length);

		super.setData(packed);
	}

}
