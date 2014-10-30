package org.echtzeitsysteme.prosem.tuio;

public class Point {
	private float x;
	private float y;
	private float angle;

	private static final float COORDS_MAX_X = 10;
	private static final float COORDS_MAX_Y = 10;

	public Point() {
		x = 0;
		y = 0;
		angle = 0;
	}

	public Point(float x, float y) {
		this.x = x;
		this.y = y;
		angle = 0;
	}

	public Point(float x, float y, float angle) {
		this.x = x;
		this.y = y;
		this.angle = angle;
	}

	public Point(final Point p) {
		this.x = p.getX();
		this.y = p.getY();
	}

	void normalize() {
		float n = getNorm();
		x /= n;
		y /= n;
	}

	public float getNorm() {
		return (float) Math.sqrt(x * x + y * y);
	}

	public float getX() {
		return x;
	}

	public int getXint() {
		return (int) (x * 32767 / (2.0 * COORDS_MAX_X));
	}

	public void setX(float x) {
		this.x = x;
	}

	public float getY() {
		return y;
	}

	public int getYint() {
		return (int) (y * 32767 / (2.0 * COORDS_MAX_Y));
	}

	public void setY(float y) {
		this.y = y;
	}

	void setXY(float x, float y) {
		this.x = x;
		this.y = y;
	}

	public float getAngle() {
		return angle;
	}

	public int getAngleInt() {
		return (int) ((angle / (2.0 * Math.PI)) * 0xFFFF);
	}

	public void setAngle(float angle) {
		this.angle = angle;
	}

	Point prod(float f) {
		return new Point(x * f, y * f);
	}

	Point div(float div) {
		return new Point(x / div, y / div);
	}

	static float prod(Point a, Point b) {
		return a.getX() * b.getX() + a.getY() * b.getY();
	}

	static Point sum(Point a, Point b) {
		return new Point(a.getX() + b.getX(), a.getY() + b.getY());
	}

	static Point diff(Point a, Point b) {
		return new Point(a.getX() - b.getX(), a.getY() - b.getY());
	}

}
