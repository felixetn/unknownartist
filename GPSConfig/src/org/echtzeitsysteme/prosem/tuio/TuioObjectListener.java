package org.echtzeitsysteme.prosem.tuio;

import TUIO.TuioObject;

public interface TuioObjectListener {
	public void tuioObjectData(TuioObject object, Point point, int camPort);
}
