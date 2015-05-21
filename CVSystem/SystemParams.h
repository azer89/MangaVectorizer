
/**
 * Some important parameters used in the pipeline
 *
 *
 * Author: Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 *
 */

#ifndef __System_Params__
#define __System_Params__

#ifndef CV_GDIM
#define CV_GDIM CV_32FC(24)
//#define CV_GDIM CV_32FC(48)
#endif

class SystemParams
{
public:
	SystemParams();
	~SystemParams();
	
public:

	// Filename
	static std::string str_filename;
	
	// Segmentation
	static int s_km_n_clusters;			// Strokes' K-Means
	static int s_km_max_iter;			// Maximum iteration to compute centroid of scribbles' gabor wavelet
	static double s_rescale_factor;
	static double s_min_size_area;

	// Gabor Wavelet (not used)
	static int g_scale;
	static int g_orientation;
	static int g_total_dim;	
	static int g_window;
	static double g_ul;
	static double g_uh;
	static int g_side;
	static int g_flag;

	// Chan-Vese IPOL Version (not used)
	static double cv_ip_Tol;
	static int	  cv_ip_MaxIter;
	static double cv_ip_Mu;
	static double cv_ip_Nu;
	static double cv_ip_Lambda1;
	static double cv_ip_Lambda2;
	static double cv_ip_dt;

	// LBP
	static int lbp_neighbors;
	static int lbp_radius;

	// Triangulation
	static double t_smooth_factor;		// for bezier interpolation 
	static double t_scale_factor;		// If you want to shrink the image [0.0 - 1.0]
	static double t_delaunay_aspect_bound;
	static double t_delaunay_max_length;

	static int    t_opticurve;
	static double t_opttolerance;

	static double t_subdivide_limit;	// keep this small
	static double t_rdp_epsilon;
	static int	  t_min_poly_area;
	static int	  t_rdp_point_min;	   // keep this small

	// Curvature Scale Space
	static int css_sigma;

	// Screentone Renderer
	static double sr_aa_value;
	static double sr_tone_frequency;
	static double sr_blur;
	static bool   sr_fixed;
	static int	  sr_blackness;

	// Rendering
	static double	  cr_blackness;

	// Cartoon+Texture Filter
	static double ct_f_sigma;
	static double ct_v_lim;

	// UI Display
	static bool gr_gpu;
	static bool gr_sc_regions;
	static bool gr_user_scribbles;
	static bool gr_show_wireframe;
	static bool gr_show_bbw_handles;
	static bool gr_show_bbw_pseudo_edge;

	// Context completion
	static int pe_num_iteration;
	static double pe_sample_distance;
	static double pe_search_radius;
	static double pe_threshold;
};

#endif