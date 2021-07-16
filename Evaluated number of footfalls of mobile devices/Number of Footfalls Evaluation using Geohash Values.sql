-- here I have converted the geohash values into respective latitude and longitudes values
SET @latitude = 30.020084;		-- Enter the input latitude 
SET @longitude = -93.147360;	-- Enter the input longitude

SELECT COUNT(DISTINCT s.ip_address) AS number_of_footfalls
FROM sample3 s			-- Use the name of the respective Sample table in the database
WHERE 
	ABS(latitude - ST_LatFromGeoHash(s.geohash)) <= s.horizontal_accuracy AND
	ABS(longitude- ST_LongFromGeoHash(s.geohash)) <= s.horizontal_accuracy 

