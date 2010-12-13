#include <Graphics2D/Filter.hh>
#include <Graphics2D/Image.hh>

#include <sys/time.h>

using namespace Graphics2D;

inline double get_real_time(const timeval& time) {
  return static_cast<double>(time.tv_sec) * 1000.0 + time.tv_usec / 1000.0;
}

void test_filter(Filter* filter, const Image& src, const std::string& file) {
  Image dst;
  dst.Init(src.GetWidth(), src.GetHeight());
  
  timeval t;
  double t_start, t_end, diff;
  gettimeofday(&t, 0);
  t_start = get_real_time(t);
  filter->FilterImage(src, dst);
  delete filter;
  gettimeofday(&t, 0);
  t_end = get_real_time(t);
  diff = t_end - t_start;
  dst.SavePPM(file);
  std::cout << "generated " << file << " in " << diff << "ms" << std::endl;
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
  Filter::Rank3x3(image, dst, 3);
  gettimeofday(&t, 0);
  t_end = get_real_time(t);
  diff = t_end - t_start;
  dst.SavePPM("out_rank.ppm");
  std::cout << "generated out_rank.ppm in " << diff << "ms" << std::endl;
  
  Filter* filter = Filter::CreateGradX();
  test_filter(filter, image, "out_gradx.ppm");

  filter = Filter::CreateGradY();
  test_filter(filter, image, "out_grady.ppm");
  
  filter = Filter::CreateLaplace();
  test_filter(filter, image, "out_laplace.ppm");
  
  dst.FillZero();
  gettimeofday(&t, 0);
  t_start = get_real_time(t);
  Filter::FilterGradMag(image, dst);
  gettimeofday(&t, 0);
  t_end = get_real_time(t);
  diff = t_end - t_start;
  dst.SavePPM("out_gradmag.ppm");
  std::cout << "generated out_gradmag.ppm in " << diff << "ms" << std::endl;
}
