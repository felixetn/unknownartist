package org.echtzeitsysteme.prosem.messages;

import java.util.LinkedHashMap;
import java.util.Set;

public class MessageClassList {
	private static LinkedHashMap<String, Class<? extends Message>> messageClassesByName = new LinkedHashMap<String, Class<? extends Message>>();
	static {
		for (Class<? extends Message> c : Message.getMessageClasses()) {
			messageClassesByName.put(c.getSimpleName(), c);
		}
	}

	public static Set<String> getMessageClassNames() {
		return messageClassesByName.keySet();
	}

	public static Class<? extends Message> getMessageClassByName(String name) {
		return messageClassesByName.get(name);
	}
	
	public static String getMessageClassNameByClass(Class<? extends Message> c) {
		for (String name : messageClassesByName.keySet()) {
			if (messageClassesByName.get(name) == c)
				return name;
		}
		return null;
	}

}
