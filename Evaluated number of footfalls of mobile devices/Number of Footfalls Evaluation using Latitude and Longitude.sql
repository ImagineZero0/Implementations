-- here I have directly used the latitude and longitudes values
SET @latitude = 30.020084;		-- Enter the input latitude 
SET @longitude = -93.147360;	-- Enter the input longitude

SELECT COUNT(DISTINCT s.ip_address) AS number_of_footfalls
FROM sample3 s			-- Use the name of the respective Sample table in the database
WHERE 
	ABS(latitude - s.latitude) <= s.horizontal_accuracy AND
	ABS(longitude- s.longitude) <= s.horizontal_accuracy 

