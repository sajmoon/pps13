package pps;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Main {

	/**
	 * @param args
	 * Number of vehicles the station can operate
	 * Max liter of Nitrogen
	 * Max liters of quantum fluids.
	 * 
	 */
	public static void main(String[] args) {
		int v, n, q, maxV;
		if (args.length != 4) {
			System.out.println("Not enough parameters.");
			System.out.println("----------------------");
			printHelpMessage();

		} else {
			try {
				printStartMessage();
				v = Integer.parseInt(args[0]);
				n = Integer.parseInt(args[1]);
				q = Integer.parseInt(args[2]);
				maxV = Integer.parseInt(args[3]);

				
				// Skapa en station
				Station s = new Station(v, n, q);
				
				
				Vehicle[] vehicles = new Vehicle[maxV];
				
				// Skapa fordon
				Random rand = new Random(19580427);
				
				
				for (int i = 0; i < maxV; i++) {
					int shipType = rand.nextInt(3); 
					vehicles[i] = new Vehicle(s, i, shipType);
					System.out.println("Create a vehicle");
					(new Thread(vehicles[i])).start();
					
				}
				
				RefuelingVehicle r1 = new RefuelingVehicle(1, s);
				RefuelingVehicle r2 = new RefuelingVehicle(2, s);
				RefuelingVehicle r3 = new RefuelingVehicle(3, s);
				RefuelingVehicle r4 = new RefuelingVehicle(4, s);
				
				(new Thread( r1 ) ).start();
				(new Thread( r2 ) ).start();
				(new Thread( r3 ) ).start();
				(new Thread( r4 ) ).start();
				
				while (true) {
					System.out.println(s.toString());
					System.out.println(r1.toString());
					System.out.println(r2.toString());
					System.out.println(r3.toString());
					System.out.println(r4.toString());
					for (int i = 0; i < maxV; i++) {
						System.out.println(vehicles[i].toString());
					}
					Thread.sleep(500);
				}
				
				
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		
		
	}

	private static void printStartMessage() {
		System.out.println("Welcome to a simulation of a fuling spacestation");
		System.out.println("Homework 4 in PPS13, KTH");
		System.out.println("-----------------------------------------");
		
	}

	private static void printHelpMessage() {
		System.out.println("Help for Fuling Spacestation Simulator");
		System.out.println("Homework 4 in PPS13, KTH");
		System.out.println("----------------------");
		System.out.println("How to use");
		System.out.println("Parameters:");
		System.out.println("1. Number of slots in the station");
		System.out.println("2. Max liters of nitrogen in the spacestation");
		System.out.println("3. Max liters of quantum fluids in spacestation");
		System.out.println("3. number of vehicles in the universe");
		
	}

}
