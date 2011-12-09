/*****************************************************************************
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * \author Mrinal Kalakrishnan <mail@mrinal.net>
 ******************************************************************************/

#ifndef ROSRT_MUTEX_H_
#define ROSRT_MUTEX_H_

#include <boost/utility.hpp>
#include <boost/assert.hpp>
#include <boost/thread/exceptions.hpp>
#include <boost/thread/locks.hpp>

#ifdef __XENO__
#include <native/mutex.h>
#else
#include <boost/thread/mutex.hpp>
#endif

namespace rosrt
{

/**
 * Wrapper for a "real-time" mutex, implementation differs based on platform.
 * Falls back to boost::mutex on generic platforms.
 *
 * Attempts to mimic the boost::mutex api, but this is not a complete implementation,
 * it's only intended for internal rosrt use.
 */
class mutex: boost::noncopyable
{
private:

#ifdef __XENO__
  RT_MUTEX mutex_;
#else
  boost::mutex mutex_;
#endif

public:

  mutex()
  {
#ifdef __XENO__
    int const res = rt_mutex_create(&mutex_, NULL);
    if (res)
    {
      throw boost::thread_resource_error();
    }
#endif
  }

  ~mutex()
  {
#ifdef __XENO__
    BOOST_VERIFY(!rt_mutex_delete(&mutex_));
#endif
  }

  void lock()
  {
#ifdef __XENO__
    int const res = rt_mutex_acquire(&mutex_, TM_INFINITE);
    BOOST_VERIFY(!res);
#else
    mutex_.lock();
#endif
  }

  void unlock()
  {
#ifdef __XENO__
    BOOST_VERIFY(!rt_mutex_release(&mutex_));
#else
    mutex_.unlock();
#endif
  }

  bool try_lock()
  {
#ifdef __XENO__
    int const res = rt_mutex_acquire(&mutex_, TM_NONBLOCK);
    BOOST_VERIFY(!res || res==EWOULDBLOCK);
    return !res;
#else
    return mutex_.try_lock();
#endif
  }

#ifdef __XENO__
  typedef RT_MUTEX* native_handle_type;
#else
  typedef boost::mutex::native_handle_type native_handle_type;
#endif

  native_handle_type native_handle()
  {
#ifdef __XENO__
    return &mutex_;
#else
    return mutex_.native_handle();
#endif
  }

  typedef boost::unique_lock<mutex> scoped_lock;
  typedef boost::detail::try_lock_wrapper<mutex> scoped_try_lock;

};

}

#endif /* ROSRT_MUTEX_H_ */