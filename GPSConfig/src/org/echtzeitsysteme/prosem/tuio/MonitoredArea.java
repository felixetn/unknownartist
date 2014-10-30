package org.echtzeitsysteme.prosem.tuio;

import TUIO.TuioClient;

public class MonitoredArea {
	private int port;
	private TuioClient client;
	private Coordinates coordinateSystem;
	private int worldPosTagBaseId;

	public MonitoredArea(int port, int worldPosTagBaseId, Point worldPosTag0,
			Point worldPosTag1, Point worldPosTag2) {
		this.port = port;
		client = new TuioClient(port);
		client.connect();

		this.worldPosTagBaseId = worldPosTagBaseId;
		coordinateSystem = new Coordinates(worldPosTag0, worldPosTag1,
				worldPosTag2);
	}

	public void disconnect() {
		client.disconnect();
	}
	
	public int getPort() {
		return port;
	}

	public TuioClient getClient() {
		return client;
	}

	public Coordinates getCoordinateSystem() {
		return coordinateSystem;
	}

	public int getWorldPosTagBaseId() {
		return worldPosTagBaseId;
	}

	public boolean isTagIdWorldCoordTag(int id) {
		return (id == worldPosTagBaseId || id == worldPosTagBaseId + 1 || id == worldPosTagBaseId + 2);
	}
}
