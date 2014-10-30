package org.echtzeitsysteme.prosem.wireless_sniffer.model.filters;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;

import org.echtzeitsysteme.prosem.messages.Message;

public class FilterQueue {

	private ArrayList<Filter> delegate = new ArrayList<Filter>();

	private HashSet<FilterChangeListener> listeners = new HashSet<FilterChangeListener>();

	public void addListener(FilterChangeListener l) {
		listeners.add(l);
	}

	public void removeListener(FilterChangeListener l) {
		listeners.remove(l);
	}

	void notifyUpdated() {
		for (FilterChangeListener l : listeners)
			l.filtersUpdated();
	}

	public Filter createFilter() {
		Filter f = new Filter(this);
		delegate.add(f);
		notifyUpdated();
		return f;
	}

	public ArrayList<Filter> getList() {
		return delegate;
	}

	/**
	 * @brief is switching the position of Filter in the filter queue. e.g.
	 *        change position from filter a (index 3) and b (index 6). After
	 *        changing filter a is on index 6 and filter b is on index 3
	 * @param from
	 *            old pos
	 * @param to
	 *            new pos
	 */
	private void switchFilterPositions(int from, int to) {
		Collections.swap(delegate, from, to);
		notifyUpdated();
	}

	/**
	 * @brief removes a filter from the gui list and filter queue
	 * @param f
	 *            to remove
	 */
	public void remove(Filter f) {
		delegate.remove(f);
		notifyUpdated();
	}

	/**
	 * @brief removes a filter from the gui list and filter queue
	 * @param index
	 *            of the filter to remove
	 */
	public void remove(int index) {
		delegate.remove(index);
		notifyUpdated();
	}

	public Filter getMatchingFilter(Message msg) {
		for (int i = 0; i < getQueueSize(); i++) {
			Filter f = get(i);
			// when the filter is enabled, then filter
			if (f.isEnabled()) {
				if (f.matchesMessage(msg))
					return f;
			}
		}
		return null;
	}

	// ##################### Getter and Setter ######################

	public int getQueueSize() {
		return delegate.size();
	}

	public Filter get(int index) {
		return delegate.get(index);
	}

	public int indexOf(Filter f) {
		return delegate.indexOf(f);
	}

	public void moveUp(Filter filter) {
		int cur = indexOf(filter);
		if (cur > 0) {
			switchFilterPositions(cur, cur - 1);
			notifyUpdated();
		}
	}

	public void moveDown(Filter filter) {
		int cur = indexOf(filter);
		if (cur < getQueueSize() - 1) {
			switchFilterPositions(cur, cur + 1);
			notifyUpdated();
		}
	}

}
