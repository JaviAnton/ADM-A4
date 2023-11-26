// include standard headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <limits.h>

// TODO: YOUR "predicated" in-place sort implementation
void swap(int* data_array, int i, int j, int direction) {
    int temp;
    if (direction == (data_array[i] > data_array[j])) {
        temp = data_array[i];
        data_array[i] = data_array[j];
        data_array[j] = temp;
    }
}

void merge(int* data_array, int low, int N, int direction) {
    if (N <= 1) {
        return;
    }

    int n = N / 2;

    for (int i = 0; i < low+n; i++)
        swap(data_array, i, i + n, direction);
    merge(data_array, low, n, direction);
    merge(data_array, low + n, n, direction);
}

void recursive_sort(int* data_array, int low, int N, int direction) {
    if (N <= 1) {
        return;
    }

    int n = N / 2;
    recursive_sort(data_array, low, n, 1);
    recursive_sort(data_array, low + n, n, 0);

    merge(data_array, low, N, direction);
}

void sort_predicated_inplace(int* data_array, int N, int direction) {
    recursive_sort(data_array, 0, N, direction);
}



// main program
int main( int argc , char ** argv ) {

	// variables for timing
	struct timeval before , after;
	long long int usec = 0;

	// check number command line arguments
	if ( argc != 3 ) {
		// incorrect number of command line arguments
		fprintf(
		    stderr ,
		    "need two arguments\n"
		);
		// exit with error status
		return( 1 );
	}

	// read first command line argument:
	// number of data values
	int N = atoi( argv[ 1 ] );
	// sanity check: echo argument value
	fprintf(
		stderr ,
		"N = %d\n" ,
		N
	);

	// read second command line argument:
	// (absolute/relative) path to input data file
	char *input_data_file_location = strdup( argv[ 2 ] );
	// sanity check: echo argument value
	fprintf(
		stderr ,
		"input_data_file_location = '%s'\n" ,
		input_data_file_location
	);

	// allocate memory for data array
	int *data_array = malloc( N * sizeof(int) );
	if ( data_array == NULL ) {
		// report error
		fprintf(
			stderr ,
			"malloc failed for data_array (%zu bytes)\n" ,
			N * sizeof(int)
		);
		// free allocated memory
		free( input_data_file_location );
		// exit with error status
		return( 1 );
	}

	// open input data file for reading
	FILE *input_data_file = fopen( input_data_file_location , "r" );
	if ( input_data_file == NULL ) {
		// report error
		perror( "fopen failed" );
		// free allocated memory
		free( data_array );
		free( input_data_file_location );
		// exit with error status
		return( 1 );
	}

	// load content of input data file in to data array;
	// measure and report time it takes
	gettimeofday( &before , NULL );
	for ( int i = 0 ; i < N ; i++ ) {
		int e = fscanf(
			input_data_file ,
			"%d\n" ,
			data_array + i
		);
		if ( e == EOF ) {
			// report error
			perror( "fscanf: premature EOF" );
			// close input data file
			fclose( input_data_file );
			// free allocated memory
			free( data_array );
			free( input_data_file_location );
			// exit with error status
			return( 1 );
		}
		if ( e < 1 ) {
			// report error
			perror( "fscanf failed" );
			// close input data file
			fclose( input_data_file );
			// free allocated memory
			free( data_array );
			free( input_data_file_location );
			// exit with error status
			return( 1 );
		}
	}
	gettimeofday( &after , NULL );
	usec = (long long int)(after.tv_sec - before.tv_sec) * 1000000
			   + (after.tv_usec - before.tv_usec);
	fprintf(
		stderr ,
		"load data: %9lld usec ; data_array[ 0 ] = %10d , data_array[ %d ] = %10d , data_array[ %d ] = %10d\n" ,
		usec , data_array[ 0 ] , N / 2 , data_array[ N / 2 ] , N - 1 , data_array[ N - 1 ]
	);

	// close input data file
	fclose( input_data_file );

	// sort data array in-place;
	// measure the time it takes
	gettimeofday( &before , NULL );
	int direction = 1;
	sort_predicated_inplace(data_array, N, direction);
	gettimeofday( &after , NULL );

	// check result correctness
	for ( int i = 1 ; i < N ; i++ ) {
		if ( data_array[ i - 1 ] > data_array[ i ] ) {
			// report error
			fprintf(
				stderr ,
				"result not sorted:\n data_array[ %d ] == %10d\n data_array[ %d ] == %10d\n" ,
				i - 1 , data_array[ i - 1 ] , i , data_array[ i ]
			);
			// free allocated memory
			free( data_array );
			free( input_data_file_location );
			// exit with error status
			return( 1 );
		}
	}

	// calculate and report sorting time
	usec = (long long int)(after.tv_sec - before.tv_sec) * 1000000
			   + (after.tv_usec - before.tv_usec);
	fprintf(
		stderr ,
		"sort data: %9lld usec ; data_array[ 0 ] = %10d , data_array[ %d ] = %10d , data_array[ %d ] = %10d\n" ,
		usec , data_array[ 0 ] , N / 2 , data_array[ N / 2 ] , N - 1 , data_array[ N - 1 ]
	);

	// output sorted data array to console (stdout)
	// (one value per line)
	for ( int i = 0 ; i < N ; i++ ) {
		printf(
			"%d\n" ,
			data_array[ i ]
		);
	}

	// free allocated memory
	free( data_array );
	free( input_data_file_location );

	// exit without error status
	return( 0 );
}