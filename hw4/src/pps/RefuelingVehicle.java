package pps;

public class RefuelingVehicle implements Runnable {
	int id;
	
	int fuel;
	int maxFuel;
	int storeFuel;
	int maxStoreFuel;
	int storeQuantum;
	int maxStoreQuantum;
	
	int distanceToStation;
	
	int position;
	int direction;
	int earth;
	
	Station station;
	
	String status;
	
	public RefuelingVehicle(int id, Station s) {
		this.id 		= id;
		fuel 			= 100;
		maxFuel			= 100;
		
		if (id == 1) {
			
			maxStoreFuel	= 5000;
			maxStoreQuantum = 0;
		} else if (id == 2) {
			maxStoreFuel	= 0;
			maxStoreQuantum = 2000;
		} else {
			// if we start more refueling ships they all get this type
			maxStoreFuel	= 1000;
			maxStoreQuantum = 1000;
		}
		
		storeFuel 		= maxStoreFuel;
		storeQuantum 	= maxStoreQuantum;
		
		station			= s;
		
		position 		= 50;
		earth			= 70; // distance to get fuel
		direction		= 0; // 0 to station 1 from station
		
		status 			= "";
		
		 
	}
	
	private void docked() {
		status = "Docked   ";
	}
	private void unloading() {
		status = "Unloading";
	}
	
	private void flyingToStation() {
		status = "on way to station";
	}
	
	private void flyingFromStation() {
		status = "on way from station";
	}
	
	private void queing() {
		status = "Queued";
	}
	
	public void run() {
		while (true) {
			try {
				if (position == 0 && direction == 0) {
					queing();
					// the refueling ship has arrived at the station
					// ready to refuel it!
					
					if (storeFuel > 0 || storeQuantum > 0) {
						if (station.giveFuel(storeFuel, storeQuantum)) {
							unloading();
							
							long time = 5;
							Thread.sleep(time *1000);
	
							storeFuel 		= 0;
							storeQuantum 	= 0;
							
							station.releasSlot();
						}
					} else {
						if (fuel > 80) {
							direction 		= 1;
						} else {
							if (station.getFuel(maxFuel - fuel, 0)) {
								docked();
								long time = 2;
								Thread.sleep(time * 1000);

								fuel = maxFuel;
								
								station.releasSlot();
								
							} else {
								// No slots availible. Waiting to fuel
								queing();
							}
						}
					}
					
				} else {
					if (direction == 0) {
						// we are on our way to the station
						
						flyingToStation();
						position--;
					} else {
						if (position == earth ) {
							direction 		= 0;
							fuel 			= maxFuel;
							storeFuel 		= maxStoreFuel;
							storeQuantum 	= maxStoreQuantum;
							flyingToStation();
							
						} else {
							// going away from station;
							flyingFromStation();
							position++;
						}
					}
					fuel--;
				}
				
				Thread.sleep(200);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	public String toString() {
		return "[Refuel-" + id + "] " + status + " " + fuel + "/" + maxFuel + "(" + storeFuel + " + " + storeQuantum + ")";
	}
}
