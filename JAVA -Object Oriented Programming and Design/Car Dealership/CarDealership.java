/**
 * <h1>Car Dealership</h1>
 * <p>
 * This program simulates a database of cars in a car dealerships inventory. The
 * program operates on database entries such as create, update, retrieve, and 
 * delete on each car.
 * 
 * @author  Alfredo Yebra Jr.
 * @authur  Ryan Askew
 * @version 1.0
 * @since   September 13, 2016
 */
package cardealership;

import java.io.FileNotFoundException;
import static java.lang.Character.getNumericValue;
import static java.lang.Character.isDigit;
import java.util.InputMismatchException;
import java.util.Scanner;

public class CarDealership {

    /**
     * Main program.
     * 
     * @param args  Array of arguments passed from command line
     */
    public static void main(String[] args) throws InputMismatchException,
            FileNotFoundException{
        int choice; // user's menu choice
        Scanner input = new Scanner(System.in); // scanner for input
        Dealership dealership = new Dealership(); // new instance of dealership

        // main loop - the program will cycle through until the program ends
        do {
            showMenu(); // show the menu
            String VIN = null;
            choice = getChoice(); // get the user's menu choice

            // depending on the user's choice, execute the following code
            switch (choice) {
                // show menu case
                case 0:
                    showMenu();
                    break;
                // show existing car records case
                case 1:
                    dealership.showAllCars(); // show the list of cars
                    break;
                // add car case 
                case 2:
                    dealership.addCar(carEntry()); // add a car to the database 
                    break;
                    // remove car by VIN case
                case 3:
                    VIN = inputVIN(); // input the desired VIN number
                    // if the car is in stock, look for its details
                    if (dealership.hasCarInstock(VIN)) {
                        // try to remove the car from the inventory
                        if (dealership.removeCar(VIN)) {
                            System.out.println("Car deleted from inventory.");
                        } 
                        // if unsuccessful for some reason, print error message
                        else {
                            System.out.print("Could not remove car from ");
                            System.out.println("inventory.");
                        }
                    }
                    // if the car was not in stock, print a message
                    else{
                        System.out.print("Cannot remove a car that is not in");
                        System.out.println(" stock.");
                    }
                    VIN = null;
                    break;
                    // search for car by VIN case
                case 4:
                    VIN = inputVIN(); // input the VIN

                    // if the car is in stock. look for it and print details
                    if (dealership.hasCarInstock(VIN)) {
                        System.out.println("Car is in stock:");
                        dealership.searchForCar(VIN);
                    } 
                    // if car is not in stock, print a message
                    else {
                        System.out.println("Car is not in stock.");
                    }
                    VIN = null;
                    break;
                    // filter by price case
                case 5:
                    dealership.sortByPrice(input); // filter cars by price
                    break;
                    // exit program case
                case 6:
                    break;
                    // if any other option is entered, display a message
                default:
                    System.out.println("Unknown menu option.");
                    System.out.println();
                    break;
            }
            // keep the program running until the user chooses to exit
        } while (choice != 6);

        // update the inventory in the file
        dealership.saveInventory();
    }

    /** Display car dealers menu options. */
    public static void showMenu() {
        System.out.println();
        System.out.println("Car Dealership Menu");
        System.out.print("1. Show all existing car records in the database ");
        System.out.println("in any order).");
        System.out.println("2. Add a new car record to the database.");
        System.out.println("3. Delete a car record from a database.");
        System.out.println("4. Search for a car (given its VIN).");
        System.out.print("5. Show a list of cars within a specified ");
        System.out.println("price range.");
        System.out.println("6. Exit program.");
        System.out.println();
    }

    /**
     * Get car dealers menu choice.
     * 
     * @return integer menu option
    */
    public static int getChoice(){
        // get the user's choice as a character
        Scanner input = new Scanner(System.in);
        System.out.print("Enter your choice: ");
        char userChoice = input.next().charAt(0);
        
        // when the user enters a non-digit, ask them again
        while (!isDigit(userChoice)){
            System.out.print("Invalid input. Enter choice from 0 to 6. ");
            userChoice = input.next().charAt(0);
        }
        
        // when the user's choice is valid, convert to int and return its value
        return getNumericValue(userChoice);
    }
    
    /**
     * Create car to CRUD in db.
     * 
     * @return Car object to be inserted into db
     */
    public static Car carEntry(){
        
        Car newCar = new Car(); // new Car object
        Scanner input = new Scanner(System.in); // input scanner
        
        String VIN = inputVIN();
        newCar.setVIN(VIN);
        
        // get the make from the user
        System.out.print("Enter make: ");
        String make = inputMake();
        newCar.setMake(make);
        
        // get the mode from the user
        System.out.print("Enter model: ");
        String model = inputModel();
        newCar.setModel(model);
        
        // get the year from the user
        System.out.print("Enter year: ");
        Integer year = inputYear();
        newCar.setYear(year);
        
        // get the mileage from the user
        System.out.print("Enter mileage: ");
        Integer mileage = inputMileage();
        newCar.setMileage(mileage);
        
        // get the price from the user
        System.out.print("Enter price: ");
        Float price = inputPrice();
        newCar.setPrice(price);
        
        System.out.println();
        
        // return the new populated Car object
        return newCar;
    }
    
    /**
     * Take and check input for the VIN number. Used for adding cars and for
     * searching for cars by VIN number.
     * 
     * @return  string representation of the VIN number.
     */
    public static String inputVIN() {
        // get the user's input for the VIN
        Scanner input = new Scanner(System.in);
        System.out.print("Enter VIN: ");
        String VIN = input.nextLine();

        // when the VIN is the wrong length, as again
        while (VIN.length() != 5) {
            System.out.println("VIN must have a length of 5");
            System.out.print("Please enter valid VIN: ");
            VIN = input.nextLine();
        }
        return VIN; // return the VIN number
    }
    
    public static String inputMake() {
        Scanner input = new Scanner(System.in);
        String make = input.nextLine();
        do{ 
            if (make.contains(" ")){
                System.out.print("Invalid input. make must be one word. "); 
                System.out.print("Try again.");
                make = input.nextLine();
            }
         }while(make.contains(" "));
        return make;
    }
    
    public static String inputModel() {
        Scanner input = new Scanner(System.in);
        String model = input.nextLine();
        
        do{      
            if (model.contains(" ")){
                System.out.print("Invalid input. Model must be one word. "); 
                System.out.print("Try again.");
                model = input.nextLine();
            }
        }while(model.contains(" "));
        return model;
    }
    
    public static Integer inputYear() {
        Scanner input = new Scanner(System.in);
        Boolean badInput = false;
        Integer year = null;
        
        do{
            try{
                badInput = false;
                year = input.nextInt();
                
                do{
                    if (year < 1900 || year > 2017){
                        System.out.print("Invalid input. Year must be "); 
                        System.out.print("between 1900 and 2017. Try again.");
                        year = input.nextInt();
                    }
                }while(year < 1900 || year > 2017);
            }
            catch(InputMismatchException e){
                badInput = true;
                
                System.out.print("Year must be an Integer. Try again.");
                input.next();
                continue;
            }
        }while(badInput == true);
        return year;
    }
    
    public static Integer inputMileage() {
        Scanner input = new Scanner(System.in);
        boolean badInput = false;
        Integer mileage = null;
        
        do{
            try{
                badInput = false;
                mileage = input.nextInt();
                
                do{
                    if (mileage < 0 || mileage > 1000000){
                        System.out.print("Invalid input. mileage must be "); 
                        System.out.print("between 0 and 1,000,000. Try again.");
                        mileage = input.nextInt();
                    }
                }while (mileage < 0 || mileage > 1000000);
            }
            catch(InputMismatchException e){
                badInput = true;
                
                System.out.print("mileage must be an Integer. Try again.");
                input.next();
                continue;
            }
        }while(badInput == true);
        return mileage;
    }
    
    public static Float inputPrice() {
        Scanner input = new Scanner(System.in);
        boolean badInput = false;
        Float price = null;
        
        do{
            try{
                badInput = false;
                price = input.nextFloat();
                
                do{
                    if (price < 0 || price > 500000){
                        System.out.print("Invalid input. Price must be "); 
                        System.out.print("between $0 and $500,000. Try again.");
                        price = input.nextFloat();
                    }
                }while (price < 0 || price > 500000);
            }
            catch(InputMismatchException e){
                badInput = true;
                
                System.out.print("price must be a decimal value. Try again.");
                input.next();
                continue;
            }
        }while(badInput == true);
        return price;
    }
    
    public static void showCar(Car car) {
        System.out.print(car.getVIN() + " ");
        System.out.print(car.getMake() + " ");
        System.out.print(car.getModel() + " ");
        System.out.print(car.getYear() + " ");
        System.out.print(car.getMileage() + " $");
        System.out.format("%1$.2f%n", car.getPrice());
    }
}
