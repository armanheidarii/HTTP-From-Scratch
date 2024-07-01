# DT065A: Implementing IoT Protocols from scratch

## Project 0
Warmup exercise for the course, create a HTTP REST Server from scratch (no REST interpreter)

#### POST
- Is used to create a sensor entity so that sensor values can be added later. 
- Returns: 
  - `201 Created` if the sensors entity was successfully created 
  - `409 Conflict` if a sensor with the given `id` already exists
  - `400 Bad Request` if there is no `id` parameter sent in the body of the request

#### PUT
- Is used to update a sensor value on the server. 
- Returns:
  - `200 OK` if the value was succesfully updated
  - `404 Not Found` if no sensor with the given `id` exists
  - `400 Bad Request` if there is no `id` parameter sent in the body of the request

#### GET
- Is used to retrieve a sensor value from the server. 
- Returns:
  - `200 OK` along with the value as a json body if the sensor was found
  - `404 Not Found` if no sensor with the given `id` exists
  - `400 Bad Request` if there is no `id` parameter sent as a url parameter

#### DELETE
- Is used to remove a sensor entity and its sensor value from the server. 
- Returns:
  - `200 OK` if the sensor was succesfully deleted
  - `404 Not Found` if no sensor with the given `id` exists
  - `400 Bad Request` if there is no `id` parameter sent as a url parameter

