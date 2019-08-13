#pragma once
#include <math.h>
#include <float.h>
#include <string.h>
#include "layer_t.h"

#pragma pack(push, 1)

// #define LEN 20

// #define Times 5

struct fc_layer_t
{
	layer_type type = layer_type::fc;
	tensor_t<float> grads_in;
	tensor_t<float> in;
	tensor_t<float> out;
	std::vector<float> input;
	tensor_t<float> weights;
	std::vector<gradient_t> gradients;

	fc_layer_t( tdsize in_size, int out_size )
		:
		in( in_size.x, in_size.y, in_size.z ),
		out( out_size, 1, 1 ),
		grads_in( in_size.x, in_size.y, in_size.z ),
		weights( in_size.x*in_size.y*in_size.z, out_size, 1 )
	{
		input = std::vector<float>( out_size );
		gradients = std::vector<gradient_t>( out_size );


		int maxval = in_size.x * in_size.y * in_size.z;

		for ( int i = 0; i < out_size; i++ )
			for ( int h = 0; h < in_size.x*in_size.y*in_size.z; h++ )
				weights( h, i, 0 ) = 2.19722f / maxval * rand() / float( RAND_MAX );
		// 2.19722f = f^-1(0.9) => x where [1 / (1 + exp(-x) ) = 0.9]
	}

	float activator_function( float x )
	{
		//return tanhf( x );
		float sig = 1.0f / (1.0f + exp( -x ));
		return sig;
	}

	float activator_derivative( float x )
	{
		//float t = tanhf( x );
		//return 1 - t * t;
		float sig = 1.0f / (1.0f + exp( -x ));
		return sig * (1 - sig);
	}

	void activate( tensor_t<float>& in )
	{
		this->in = in;
		activate();
	}

	int map( point_t d )
	{
		return d.z * (in.size.x * in.size.y) +
			d.y * (in.size.x) +
			d.x;
	}

	void activate()
	{
		#ifdef Times
		FILE *fp = NULL; 
	   	fp = fopen("a.txt", "a+");
		#endif 	
		for ( int n = 0; n < out.size.x; n++ )
		{
			float inputv = 0;

			for ( int i = 0; i < in.size.x; i++ )
				for ( int j = 0; j < in.size.y; j++ )
					for ( int z = 0; z < in.size.z; z++ )
					{
						int m = map( { i, j, z } );
						mul++;
						string str = FloatToByBinaryByAlgo1(in( i, j, z )).substr(0,LEN)+FloatToByBinaryByAlgo1(weights( m, n, 0 )).substr(0,LEN);
						if(M[str]){
							inputv += m1[str];
							hit++;
						}						
						else{
							inputv += in( i, j, z ) * weights( m, n, 0 );
						}	
						//inputv += in( i, j, z ) * weights( m, n, 0 );
						
						#ifdef Times
									fprintf(fp, "%.32f %.32f %.32f\n",in( i, j, z ) , weights( m, n, 0 ),in( i, j, z ) * weights( m, n, 0 ));
						#endif 
					}

			input[n] = inputv;

			out( n, 0, 0 ) = activator_function( inputv );
		}
		#ifdef Times
			fclose(fp);
		#endif 
	}

	void fix_weights()
	{
		for ( int n = 0; n < out.size.x; n++ )
		{
			gradient_t& grad = gradients[n];
			for ( int i = 0; i < in.size.x; i++ )
				for ( int j = 0; j < in.size.y; j++ )
					for ( int z = 0; z < in.size.z; z++ )
					{
						int m = map( { i, j, z } );
						float& w = weights( m, n, 0 );
						w = update_weight( w, grad, in( i, j, z ) );
					}

			update_gradient( grad );
		}
	}

	void calc_grads( tensor_t<float>& grad_next_layer )
	{
		memset( grads_in.data, 0, grads_in.size.x *grads_in.size.y*grads_in.size.z * sizeof( float ) );
		#ifdef Times
			FILE *fp = NULL; 
	    	fp = fopen("a.txt", "a+");
		#endif 
		for ( int n = 0; n < out.size.x; n++ )
		{
			gradient_t& grad = gradients[n];
			grad.grad = grad_next_layer( n, 0, 0 ) * activator_derivative( input[n] );
			
			#ifdef Times
				fprintf(fp,"%.32f %.32f %.32f\n",grad_next_layer( n, 0, 0 ),activator_derivative( input[n] ),grad_next_layer( n, 0, 0 ) * activator_derivative( input[n] ));
			#endif 
			
			for ( int i = 0; i < in.size.x; i++ )
				for ( int j = 0; j < in.size.y; j++ )
					for ( int z = 0; z < in.size.z; z++ )
					{
						int m = map( { i, j, z } );

						mul++;
						string str = FloatToByBinaryByAlgo1(grad.grad ).substr(0,LEN)+FloatToByBinaryByAlgo1(weights( m, n, 0 )).substr(0,LEN);
						if(M[str]){
							grads_in( i, j, z ) += m1[str];
							hit++;
						}						
						else{
							grads_in( i, j, z ) += grad.grad * weights( m, n, 0 );
						}	
						// grads_in( i, j, z ) += grad.grad * weights( m, n, 0 );
						
						#ifdef Times
									fprintf(fp,"%.32f %.32f %.32f\n",grad.grad ,weights( m, n, 0 ),grad.grad * weights( m, n, 0 ));
						#endif 
					}
		}
		#ifdef Times
			fclose(fp);
		#endif 
		
	}
};
#pragma pack(pop)
