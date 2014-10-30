package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425;

public interface SettingChangeListener {
	public void wirelessSettingsChanged(Amber8425 src, int rfId, int channel);
}
