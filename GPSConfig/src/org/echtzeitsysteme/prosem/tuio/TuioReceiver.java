package org.echtzeitsysteme.prosem.tuio;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Vector;

import TUIO.TuioObject;

public class TuioReceiver implements Runnable {
	private HashMap<Integer, MonitoredArea> areas = new HashMap<Integer, MonitoredArea>();
	private Thread runThread = null;
	private boolean run = true;

	private HashSet<TuioObjectListener> listeners = new HashSet<TuioObjectListener>();

	private final int TIMEOUT = 1000;

	public TuioReceiver() {
		runThread = new Thread(this);
		runThread.start();
	}

	public synchronized void stop() {
		if (runThread != null) {
			run = false;
			try {
				runThread.join();
			} catch (InterruptedException e) {
			}
			runThread = null;
		}
	}

	public void addMonitoredArea(int port) {
		if (!areas.containsKey(port)) {
			areas.put(port, new MonitoredArea(port, 0, new Point(0f, 0f),
					new Point(1f, 0f), new Point(0f, 1f)));
		}
	}

	public void closeMonitoredArea(int port) {
		MonitoredArea a = areas.get(port);
		if (a != null) {
			a.disconnect();
		}
	}
	
	public void closeAllMonitoredAreas() {
		for (MonitoredArea ma : areas.values())
			ma.disconnect();
	}

	public void addListener(TuioObjectListener l) {
		listeners.add(l);
	}

	public void removeListener(TuioObjectListener l) {
		listeners.remove(l);
	}

	private void notifyListeners(TuioObject obj, Point point, int port) {
		for (TuioObjectListener l : listeners)
			l.tuioObjectData(obj, point, port);
	}

	@Override
	public void run() {
		while (run) {
			try {
				for (MonitoredArea area : areas.values()) {
					Vector<TuioObject> objects = area.getClient()
							.getTuioObjects();

					int updates = 0;

					Iterator<TuioObject> objectsIt = objects.iterator();
					while (objectsIt.hasNext()) {
						TuioObject obj = objectsIt.next();
						int id = obj.getSymbolID();
						if (area.isTagIdWorldCoordTag(id)) {
							area.getCoordinateSystem().updateFixPoint(
									id - area.getWorldPosTagBaseId(),
									obj.getX(), obj.getY());

							objectsIt.remove();

							updates++;
							if (updates >= 3)
								break;
						}
					}

					if (updates > 0)
						area.getCoordinateSystem().updateCoordSystem();

					// CALC WORLDCOORDINATES
					for (TuioObject obj : objects) {
						Point world = area.getCoordinateSystem()
								.getWorldCoords(obj.getX(), obj.getY(),
										obj.getAngle());

						notifyListeners(obj, world, area.getPort());

						System.out
								.println(String
										.format(" Cam: %d\tCar: %d\tX: %f(%d)\tY: %f(%d)\tAngle: %f(%d)\tRX: %f\tRY: %f\tRAngle: %f",
												0, obj.getSymbolID(),
												world.getX(), world.getXint(),
												world.getY(), world.getYint(),
												world.getAngle(),
												world.getAngleInt(), 0f, 0f, 0f));
					}
				}
				Thread.sleep(TIMEOUT);
			} catch (InterruptedException e) {
			}
		}
	}

}
