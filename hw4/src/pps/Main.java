package pps;

public class Main {

	/**
	 * @param args
	 * Number of vehicles the station can operate
	 * Max liter of Nitrogen
	 * Max liters of quantum fluids.
	 * 
	 */
	public static void main(String[] args) {
		int v, n, q;
		if (args.length != 3) {
			System.out.println("Not enough parameters.");
			System.out.println("----------------------");
			printHelpMessage();

		} else {
			try {
				printStartMessage();
				v = Integer.parseInt(args[0]);
				n = Integer.parseInt(args[1]);
				q = Integer.parseInt(args[2]);

				Station s = new Station(v, n, q);
				
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
		System.out.println("1. Number of vehicles");
		System.out.println("2. Max liters of nitrogen in the spacestation");
		System.out.println("3. Max liters of quantum fluids in spacestation");
		
	}

}
