/**
 * <h1>Car Dealership class</h1>
 * 
 * @authur  Alfredo Yebra Jr.
 * @authur  Ryan Askew
 * @version 1.0
 * @since   September 12, 2016
 */
package cardealership;

import static cardealership.CarDealership.showCar;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.List;
import java.util.Scanner;

public class Dealership {
    final private List<Car> inventory; // list of cars in inventory

    /** Constructor. */
    public Dealership() {
        inventory = new ArrayList<>(); // create empty arraylist
        processInventory(); // process the existing entries in the file
    }

    /**
     * Add car and its information to inventory db.
     * 
     * @param   car object to insert to database.
     * @return  <code>true</code> if car was successfully added to inventory db.
     */
    public boolean addCar(Car car) {
        // add the Car object to the list
        return inventory.add(car);
    }
    
    /**
     * Remove a car from the dealership inventory.
     *
     * @param car   Car to be deleted from db.
     * @return      <code>true</code> if the car has been removed from 
     * inventory.
     */
    public boolean removeCar(String VIN) {
        // remove the car that matches the given VIN number
       return inventory.removeIf((Car i) -> {
            return i.getVIN().equals(VIN);
        });
    }
    
    /** Display all cars in currently in inventory. */
    public void showAllCars() {
        // display a message if the list is empty
        if(inventory.isEmpty()){
            System.out.println("There are no cars in the inventory");
        }
        // otherwise, show all the cars in the list
        else{
            System.out.println("\nDealership Inventory\n"); // title
            
            // traverse the list and print each entry, formatting properly
            for (int i = 0; i < inventory.size(); i++){
                showCar(inventory.get(i));
            }
        }
    }

    /**
     * Check if car is in inventory by searching for VIN.
     * 
     * @param   VIN vehicle identification number.
     * @return  <code>true</code> car is found.
     */
    public boolean hasCarInstock(String VIN) {
        // check whether a car is in stock that matches the given VIN number
        return inventory.stream().anyMatch(car -> {
            return car.getVIN().contains(VIN);
        });
    }
    
    /**
     * search for a car by its VIN number, then display its details.
     * 
     * @param   String object is the VIN number to search for
     */
    public void searchForCar(String VIN){
        // traverse the list
        for (int i = 0; i < inventory.size(); i++){
            // look for a car that matches the given VIN number, and show its
            // details, making sure they are formatted properly
            if (inventory.get(i).getVIN().equals(VIN)){
                showCar(inventory.get(i));
            }
        }
    }
    /**
     * takes a price range and displays list of cars within that price range
     * 
     * @param   input variable (scanner) to get input for lower/upper bounds
     * 
     * @throws InputMismatchException
     */
    public void sortByPrice(Scanner input) throws InputMismatchException{
        // declare lower and upper price bounds
        float lower, upper;
        // try to get these values from the user
        try{
            // get lower price bound. ask again if value is out of range
            do{
                System.out.print("Enter lower price bound: ");
                lower = input.nextFloat();

                if (lower < 0){
                    System.out.print("Invalid input: lower bound must be at ");
                    System.out.print("least 0. Try again: ");
                    lower = input.nextFloat();
                }
            }while(lower < 0);
        }
        // exception when user enters values other than integers - ask again
        catch(InputMismatchException e){
            input.next();
            System.out.print("Please enter a decimal value or a whole number.");
            lower = input.nextFloat();
        }
        
        // same block of code as for lower bound, but this time asking for upper
        // bound
        try{
            do{
                System.out.print("Enter upper price bound: ");
                upper = input.nextFloat();

                if (upper < lower){
                    System.out.print("Invalid input: upper bound must be at ");
                    System.out.print("least equal to lower bound. Try again: ");
                    upper = input.nextFloat();
                }
            }while(upper < lower);
        }
        catch(InputMismatchException e){
            input.next();
            System.out.print("Please enter a decimal value or a whole number.");
            upper = input.nextFloat();
        }
        
        // store the final values for lower/upper bounds
        final float lowerFinal = lower, upperFinal = upper;
        
        // header
        System.out.print("All cars priced between $");
        System.out.format("%1$.2f", lowerFinal);
        System.out.print(" and $");
        System.out.format("%1$.2f", upperFinal);
        System.out.println(":");
        
        // count keeps track of how many entries have been printed so far
        int count = 0;
        
        // traverse the list
        for (int i = 0; i < inventory.size(); i++){
            // print all entries that fall within the specified price range
            if (inventory.get(i).getPrice() >= lowerFinal &&
                    inventory.get(i).getPrice() <= upperFinal){
               showCar(inventory.get(i));
               // increment count to account for each entry that is printed
               count++;
            }
        }
        // if no cars are in the price range, display a message
        if (count == 0)
            System.out.println("No cars in this price range.\n");
    }

    /**
     * Converts the .txt file containing the dealership's inventory into an
     * ArrayList.
     */
    private void processInventory() {
        // try reading the list from the file
        try {
            List<String> carFileData = 
                    Files.readAllLines(Paths.get("./cars.txt"));
            for (String carData: carFileData) {
                String[] c = carData.split("\\W+");
                inventory.add(new Car(
                        c[0], // VIN
                        c[1], // Make
                        c[2], // Model
                        Integer.parseInt(c[3]), // Year
                        Integer.parseInt(c[4]), // Mileage
                        Float.parseFloat(c[5])) // Price
                ); 
            }
        } 
        // exception when there is something wrong with the input file
        catch (IOException e) {
            e.printStackTrace(System.out);
        }
    }
    
        /** Write and save dealerships inventory file data. */
    public void saveInventory() {
        // use the public interface to access a private member function that
        // will be able to update the file containing the inventory
        updateInventory();
    }
    
        /**
     * Updates the inventory list that is stored in the file to reflect changes
     * made while the program was running.
     */
    private void updateInventory() {
        // create a new array list to store the formatted output
        List<String> savedFile = new ArrayList<>();

        // try storing all of the entries in the file
        try {
            // traverse the list
            for (int i = 0; i < inventory.size(); i++){
                // use a String object to store each formatted object before
                // it is written to the file
                String tempCar = inventory.get(i).toString();
                        /*inventory.get(i).getVIN() + " " +
                        inventory.get(i).getMake() + " " +
                        inventory.get(i).getModel() + " " +
                        inventory.get(i).getYear().toString() + " " +
                        inventory.get(i).getMileage().toString() + " " +
                        String.format("%1$.2f", inventory.get(i).getPrice());*/
                // add the string of each object to the list
                savedFile.add(tempCar);
            }
            
            // write the entire list to the file
            Files.write(Paths.get("./cars.txt"), savedFile);
        } 
        // exception if there is something wrong with the input or the file
        catch (IOException e) {
            e.printStackTrace(System.out);
        }
    }
}

