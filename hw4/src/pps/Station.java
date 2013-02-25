package pps;

public class Station {

	int maxFuel;
	int maxQuantum;
	int fuel;
	int quantum;
	
	int maxSlots;
	int freeSlots;
	
	public Station(int v, int n, int q) {
		maxFuel = n;
		fuel = n;
		
		maxQuantum = q;
		quantum = q;
		
		maxSlots = v;
		freeSlots = v;
	}
	
	private synchronized boolean changeFuel(int f, int q) {
		fuel += f; quantum += q;
		return true;
	}
	
	public synchronized boolean getFuel(int f, int q) {
		if (hasFreeSlot()) {
			if (fuel >= f && quantum >= q) {
				if (changeFuel(-f, -q)) {
					return true;
				} else {
					releasSlot();
				}
			}
		}
		return false;
	}
	
	public synchronized boolean giveFuel(int f, int q) {
		if (hasFreeSlot()) {
			if ( (maxFuel - fuel) > f && ( maxQuantum - quantum) > q) {
				if (changeFuel(f, q)) {
					return true;
				}
			} else {
				releasSlot();
			}
		}
		return false;
	}
	
	public synchronized void releasSlot() {
//		System.out.println("Ship leaving slot");
		freeSlots++;	
	}
	
	private boolean hasFreeSlot() {
		if (freeSlots > 0) {
//			System.out.println("Ship docked in slot");
			freeSlots--;
			return true;
		}
		return false;
	}

	public String toString() {
		return "[Station] " + fuel + "/" + maxFuel + " " + quantum + "/" + maxQuantum + " slots: " + freeSlots + "/" + maxSlots;
	}

}
