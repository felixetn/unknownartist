package org.echtzeitsysteme.prosem.tuio;

public class Coordinates {

	private Point[] worldPosTag = new Point[3];
	private Point[] camPosTag = new Point[3];

	private float weight;
	private Point na = new Point();
	private Point nb = new Point();
	private Point nc = new Point();
	private float da;
	private float db;
	private float dc;
	private float angleOffset;

	public Coordinates(Point worldPosTag0, Point worldPosTag1,
			Point worldPosTag2) {
		worldPosTag[0] = worldPosTag0;
		worldPosTag[1] = worldPosTag1;
		worldPosTag[2] = worldPosTag2;
		for (int i = 0; i < 3; i++)
			camPosTag[i] = new Point();
	}

	/**
	 * Used to update one fixpoint of a Cluster
	 * 
	 * @param worldTag
	 *            Number of tag within the coordinate system (0-2)
	 * @param x
	 *            cam x coord
	 * @param y
	 *            cam y coord
	 */
	public void updateFixPoint(int worldTag, float x, float y) {
		if ((worldTag < 0) || (worldTag > 2))
			return;
		camPosTag[worldTag].setXY(x, y);
	}

	/**
	 * updates the cluster and recalcs all needed data
	 */
	public void updateCoordSystem() {
		// update fixpoints
		weight = area(camPosTag[0], camPosTag[1], camPosTag[2]);

		// calc center point
		Point center = Point.sum(Point.sum(camPosTag[0], camPosTag[1]),
				camPosTag[2]).div(3.0f);

		// calc normals and normalize
		Point va = Point.diff(camPosTag[1], camPosTag[2]);
		Point vb = Point.diff(camPosTag[2], camPosTag[0]);
		Point vc = Point.diff(camPosTag[0], camPosTag[1]);

		na.setXY(-va.getY(), va.getX());
		na.normalize();

		nb.setXY(-vb.getY(), vb.getX());
		nb.normalize();

		nc.setXY(-vc.getY(), vc.getX());
		nc.normalize();

		// Hesse normal form
		da = Point.prod(na, camPosTag[1]);
		db = Point.prod(nb, camPosTag[2]);
		dc = Point.prod(nc, camPosTag[0]);

		// correct direction of normal vector. Vector has to point into the
		// triangle!
		float aa = sgn(Point.prod(na, center) - da);
		float ab = sgn(Point.prod(nb, center) - db);
		float ac = sgn(Point.prod(nc, center) - dc);

		na = na.prod(aa);
		nb = nb.prod(ab);
		nc = nc.prod(ac);

		da = da * aa;
		db = db * ab;
		dc = dc * ac;

		Point X = Point.diff(camPosTag[1], camPosTag[0]);

		if (X.getY() > 0)
			angleOffset = (float) (Math.acos(-X.getX() / X.getNorm()) + Math.PI);
		else
			angleOffset = (float) Math.acos(X.getX() / X.getNorm());
	}

	public Point getWorldCoords(float x, float y, float angle) {
		float L0, L1, L2, worldAngle;

		// create point
		Point p = new Point(x, y, angle);

		// calc barycentric coordinates
		L0 = area(p, camPosTag[1], camPosTag[2]) / weight;
		L1 = area(p, camPosTag[0], camPosTag[2]) / weight;
		L2 = area(p, camPosTag[0], camPosTag[1]) / weight;

		// correct coordinates, if point is outside of triangle
		float aa = sgn(Point.prod(na, p) - da);
		float ab = sgn(Point.prod(nb, p) - db);
		float ac = sgn(Point.prod(nc, p) - dc);

		L0 *= aa;
		L1 *= ab;
		L2 *= ac;

		// calc worldangle
		worldAngle = -angleOffset - angle;

		while (worldAngle < 0)
			worldAngle += 2 * Math.PI;

		while (worldAngle >= 2 * Math.PI)
			worldAngle -= 2 * Math.PI;

		// calc coordinates
		return new Point(L0 * worldPosTag[0].getX() + L1
				* worldPosTag[1].getX() + L2 * worldPosTag[2].getX(), L0
				* worldPosTag[0].getY() + L1 * worldPosTag[1].getY() + L2
				* worldPosTag[2].getY(), worldAngle);
	}

	/**
	 * Calculates the relative area amount of point a in comparison to point b
	 * and c
	 */
	private float area(Point a, Point b, Point c) {
		Point r = new Point(b.getX() - a.getX(), b.getY() - a.getY());
		Point t = new Point(c.getX() - a.getX(), c.getY() - a.getY());

		return Math.abs((float) (0.5 * (r.getX() * t.getY() - r.getY()
				* t.getX())));
	}

	private float sgn(float f) {
		return (f < 0 ? -1 : 1);
	}
}
