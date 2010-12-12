#include <Graphics2D/Filter.hh>
#include <Graphics2D/Image.hh>

#include <sys/time.h>

using namespace Graphics2D;

inline double get_real_time(const timeval& time) {
  return static_cast<double>(time.tv_sec) * 1000.0 + time.tv_usec / 1000.0;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "usage: " << argv[0] << " src.ppm" << std::endl;
    return 0;
  }
  
  Image image;
  if (argc >= 2)
    image.LoadPPM(argv[1]);
  
  Image dst;
  dst.Init(image.GetWidth(), image.GetHeight());
  
  timeval t;
  double t_start, t_end, diff;
  // Build mean filtered image
  gettimeofday(&t, 0);
  t_start = get_real_time(t);

  Filter* filter = Filter::CreateMean(7, 7);
  filter->FilterImage(image, dst);
  delete filter;
  gettimeofday(&t, 0);
  t_end = get_real_time(t);
  diff = t_end - t_start;
  dst.SavePPM("src_meanA.ppm");
  std::cout << "generated src_meanA.ppm in " << diff << "ms" << std::endl;
  
  // Build seperatable mean filtered image
/*  dst.FillZero();
  gettimeofday(&t, 0);
  t_start = get_real_time(t);
  filter = Filter::CreateMean(7, 1);
  filter->FilterImage(image, dst);
  delete filter;
  
  filter = Filter::CreateMean(1, 7);
  filter->FilterImage(dst, dst);
  delete filter;
  gettimeofday(&t, 0);
  t_end = get_real_time(t);
  diff = t_end - t_start;
  dst.SavePPM("src_meanB.ppm");
  std::cout << "generated src_meanB.ppm in " << diff << "ms" << std::endl;
  
  // Build binomial filtered image
  dst.FillZero();
  gettimeofday(&t, 0);
  t_start = get_real_time(t);
  filter = Filter::CreateBinomial(7);
  filter->FilterImage(image, dst);
  delete filter;
  gettimeofday(&t, 0);
  t_end = get_real_time(t);
  diff = t_end - t_start;
  dst.SavePPM("src_binom.ppm");
  std::cout << "generated src_binom.ppm in " << diff << "ms" << std::endl;
  */
  
  //filter using recursive filter
  dst.FillZero();
  gettimeofday(&t, 0);
  t_start = get_real_time(t);
  Graphics2D::Filter::MeanRecursive(image,dst,7,7);
  gettimeofday(&t, 0);
  t_end = get_real_time(t);
  diff = t_end - t_start;
  dst.SavePPM("src_7x7_recursive.ppm");
  std::cout << "generated src_7x7_recursive.ppm in " << diff << "ms" << std::endl;
  
  //filter using recursive filter
  dst.FillZero();
  gettimeofday(&t, 0);
  t_start = get_real_time(t);
  Filter::MeanRecursive(image,dst,31,31);
  gettimeofday(&t, 0);
  t_end = get_real_time(t);
  diff = t_end - t_start;
  dst.SavePPM("src_31x31_recursive.ppm");
  std::cout << "generated src_31x31_recursive.ppm in " << diff << "ms" << std::endl;
  
  
}
