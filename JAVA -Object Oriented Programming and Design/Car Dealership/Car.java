package cardealership;

import java.util.InputMismatchException;
import java.util.Scanner;

public class Car {
    private String VIN; // VIN number of car
    private String make; // make of car
    private String model; // model of car
    private Integer year; // year of car 
    private Integer mileage; // mileage of car
    private Float price; // price of car

    /**
     * Default constructor, sets year to 1980, price to 0, everything else
     * to empty.
     */
    public Car() {
        VIN = "";
        make = "";
     	model = "";
     	year = 1980;
     	mileage = 0;
     	price = 0.0f;
    }

    /**
     * Parameterized constructor.
     * 
     * @param _VIN      Vehicle identification number.
     * @param _make     Make of vehicle.
     * @param _model    Model of vehicle.
     * @param _year     Year of vehicle.
     * @param _mileage  Miles driven by vehicle used.
     * @param _price    Price of vehicle in dollars.
     */
    public Car(String _VIN, String _make, String _model,
            Integer _year, Integer _mileage, Float _price) {
        VIN = _VIN;
        make = _make;
        model = _model;
        year = _year;
        mileage = _mileage;
        price = _price;
    }
    
    /**
     * Sets a Car object's VIN number.
     * 
     * @param   String object represents the VIN number.
     */
    public void setVIN(String _VIN) {
        VIN = _VIN;
    }
    
    /**
     * Sets a Car object's make.
     * 
     * @param   String object represents the make.
     */
    public void setMake(String _make) {
        make = _make;
    }
    
    /**
     * Sets a Car object's model.
     * 
     * @param   String object represents the model.
     */
    public void setModel(String _model) {
        model = _model;
    }
    
    /**
     * Sets a Car object's year.
     */
    public void setYear(Integer _year){
        year = _year;
           
     
    }
    
    /**
     * Sets a Car object's mileage.
     */
    public void setMileage(Integer _mileage) {
        mileage = _mileage;
    }
    
    /**
     * Sets a Car object's price.
     */
    public void setPrice(Float _price) {
        price = _price;
    }
    
    /**
     * gets the car object's VIN
     * 
     * @return  String VIN number
     */
    protected String getVIN() {
        return VIN;
    }
    
        /**
     * gets the car object's make
     * 
     * @return  String make
     */
    public String getMake() {
        return make;
    }
    
        /**
     * gets the car object's model
     * 
     * @return  String model
     */
    public String getModel() {
        return model;
    }
    
        /**
     * gets the car object's year
     * 
     * @return  Integer year
     */
    public Integer getYear() {
        return year;
    }

        /**
     * gets the car object's mileage
     * 
     * @return  Integer mileage
     */
    public Integer getMileage() {
        return mileage;
    }
    
        /**
     * gets the car object's price
     * 
     * @return  Float price
     */
    public Float getPrice() {
        return price;
    }
    
    /**
     * Return car object as String.
     * 
     * @return String 
     */
    @Override
    public String toString() {
        String s = " ";
        return VIN + s + make + s + model + s + year + s + mileage + s + 
                String.format("%1$.2f%n", price);
    }
}
