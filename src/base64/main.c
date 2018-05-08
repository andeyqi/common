#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"

int
main(void)
{
	unsigned char input[] = "iVBORw0KGgoAAAANSUhEUgAAAQQAAAEEAQAAAADmc3enAAABmklEQVR42u2ZMbKDMAxE5XGRMkfIUXw0OBpH4QiUKTLW164ghP+TSftnBxdxgFctklYy5l/WYidxEifxD4nFsC7u08B/NpQH9yJJ3OLncVlu0+B+v842xBW0ECUmK+s9bJOBhyzCBO5FECzWvOgTPjfvIOyjYhpExrq10erdIuTLh2yQILKOLfkot7eVToLY1hyv3anHJxdTIKAHFWijP+zqjiC4znGlSMQae3U4cr72SGnIcnj7QsRkPfsP6BHlLCgasyLBOg0Fno6cVjUWRSLMafzlyNF0vVZtKYIX1GN1ZKurSIIE+g/fXOm+Z/YgSaDNKpngmBswLFUWt6JJWOoBj7qw96IsJkmgZHVG97D2ltwOvaUQcYNHofGIzKYj98ogcE1i7z8ypZngzWUJzoTrIQ77Dzt4thDxnBDaOj5g9F0OVVuI2GZCKBB67KOhJpHzPlyJHkVzAi9KbCdXXMFzm96dbekQeFRZwNBbvs6EokSv2Xgw5M01if18PTc41vw3GzSIvY51tlkjpv+5uSRxfgU8iZNQI34An+fd5bQtGOAAAAAASUVORK5CYII=";
	unsigned char decode[1000] = {0};
	//char output[13];
	//bzero(output, sizeof(output));
	//base64_encode(input, sizeof(input) - 1, output);
	base64_decode(input, sizeof(input) - 1, decode);

	return 0;
}

