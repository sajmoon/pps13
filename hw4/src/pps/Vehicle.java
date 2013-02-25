package pps;

public class Vehicle implements Runnable {
	
	int id;
	int maxFuel;
	int fuel;
	int maxQuantum;
	int quantum;
	
	int type;
	
	String ident;
	
	Station station;
	
	String status;
	
	public Vehicle(Station s, int id, int t) {
		this.id 	= id;
		maxFuel 	= 100;
		fuel 		= 20;
		maxQuantum 	= 100;
		quantum 	= 70;
		station 	= s;
		
		type = t;
		
		
		ident = getIdent();
		flying();
		
	}
	
	private void docked() {
		status = "Docked";
	}
	
	private void flying() {
		status = "Flying";
	}
	
	private void queing() {
		status = "Queued";
	}
	
	
	
	public void run() {
		System.out.println("Start enging on vehicle " + id);
		while (true) {
			try {
				if (fuel < (10) || quantum < 10) {
					// if 10 fuel left, try to refuel
					if (station.getFuel(maxFuel - fuel, maxQuantum - quantum)) {
						docked();
						long time = 2;
						Thread.sleep(time * 1000);

						fuel = maxFuel;
						quantum = maxQuantum;
						station.releasSlot();
						
					} else {
						// No slots availible. Waiting to fuel
						queing();
					}
				} else {
					// Flying
					flying();
					fly();
				}
				
				Thread.sleep(200);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	private void fly() {
		
		if (type == 0)
			fuel -= 1;
		if (type == 1) {
			fuel--;
			quantum--;
		}
		if (type == 2) 
			quantum--;
		
			
	}
	private String getType() {
		if (type == 0)
			return " Small  f  ";
		else if (type == 1)
			return " Fast   f+q";
		else if (type == 2)
			return " Stealt   q";
		
		return "Waat?";
	}
	
	private String getID() {
		if (id < 10)
			return " " + id;
		if (id < 100)
			return "" + id;
		return "null";
	}
	
	private String getIdent() {
		return "[" + getID() + getType() + "] ";
	}
	
	private String getStatus() {
		return status;
	}
	
	private String getFuel() {
		String o = "  ";
		
		if (fuel < 10)
			o += "00" + fuel;
		else if (fuel < 100)
			o += "0" + fuel;
		else if (fuel < 1000)
			o += fuel;
		
		o += "/" + maxFuel + " & ";
		
		if (quantum < 10)
			o += "00" + quantum;
		else if (quantum < 100)
			o += "0" + quantum;
		else if (quantum < 1000)
			o+= "" + quantum;
		
		o += "/" + maxQuantum;
			
		return o;
	}
	public String toString() {
		return getIdent() + getStatus() + getFuel();

	}
}
