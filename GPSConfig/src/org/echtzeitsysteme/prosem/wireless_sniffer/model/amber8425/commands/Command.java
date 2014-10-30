package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Date;
import java.util.HashMap;

public class Command {
	public static final int START_BYTE = 0x02;

	private static HashMap<Integer, Class<? extends Command>> commandsById = new HashMap<Integer, Class<? extends Command>>();
	static {
		commandsById.put(0x01, CMD_DATAEX_REQ.class);
		commandsById.put(0x0A, CMD_GET_REQ.class);
		commandsById.put(0x40, CMD_DATA_REPLY.class);
		commandsById.put(0x49, CMD_SET_REPLY.class);
		commandsById.put(0x4A, CMD_SET_REQ.class); // CMD_GET_REPLY is same
													// format as SET_REQ
		commandsById.put(0x81, CMD_DATAEX_IND.class);
	}

	public enum EState {
		RECV,
		NOT_SENT,
		GOT_REPLY,
		TIMEOUT
	};

	private int commandId;
	private int[] payload;

	private Command reply = null;
	private EState state = EState.RECV;

	private Date date;

	protected Command(boolean out, int commandId) {
		this.date = new Date();
		this.commandId = commandId;
		if (out)
			state = EState.NOT_SENT;
	}

	protected void setData(int[] data) {
		payload = data;
	}

	public void setReply(Command reply) {
		this.reply = reply;
		if (reply != null)
			state = EState.GOT_REPLY;
		else
			state = EState.TIMEOUT;
	}

	public int getCommandId() {
		return commandId;
	}

	public int[] getData() {
		return payload;
	}

	public int[] getCommandData() {
		int[] raw = new int[1 + 1 + 1 + payload.length + 1]; // Start byte,
																// command,
		// length, data,
		// checksum
		raw[0] = START_BYTE;
		raw[1] = commandId & 0xff;
		raw[2] = payload.length & 0xff;
		System.arraycopy(payload, 0, raw, 3, payload.length);

		int check = 0;
		for (int i = 0; i < (raw.length - 1); i++)
			check ^= raw[i];
		raw[raw.length - 1] = check & 0xff;

		return raw;
	}

	public Date getDate() {
		return date;
	}

	public boolean isDirectionOut() {
		return state != EState.RECV;
	}

	public EState getState() {
		return state;
	}

	public Command getReply() {
		return reply;
	}

	public void printCommand() {
		System.out.println();
		System.out.println("Command:");
		System.out.println("  Id:     " + commandId);
		System.out.println("  DirOut: " + isDirectionOut());
		System.out.println("  Date:   " + getDate());
		System.out.println("  State:  " + state);
		System.out.print("  Data:   ");
		for (int i : payload) {
			System.out.print(String.format("%02x:", i));
		}
		System.out.println();
		System.out.println();
	}

	public static Command parseCommand(int[] data) {
		int commandId = data[1];
		int dataLength = data[2];
		int[] payload = new int[dataLength];
		System.arraycopy(data, 3, payload, 0, dataLength);

		if (commandsById.containsKey(commandId)) {
			Class<? extends Command> c = commandsById.get(commandId);
			try {
				Method m = c.getMethod("parseRaw", int.class, int[].class);
				Object o = m.invoke(null, commandId, payload);
				if (o instanceof Command)
					return (Command) o;
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
		Command cmd = new Command(false, commandId);
		cmd.setData(payload);
		return cmd;
	}
}
