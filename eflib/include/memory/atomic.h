#ifndef EFLIB_MEMORY_ATOMIC_H
#define EFLIB_MEMORY_ATOMIC_H

#include <eflib/include/platform/config.h>

namespace eflib{
	class spinlock {
	private:
		typedef enum {Locked, Unlocked} LockState;
		LockState state_;

	public:
		spinlock() : state_(Unlocked) {}
		
		bool try_lock()
		{
			auto old_state = state_;
			state_ = Locked;
			return old_state == Unlocked;
		}

		void lock()
		{
			while ( !try_lock() )
			{
				/* busy-wait */
			}
		}

		void unlock()
		{
			state_ = Unlocked;
		}
	};

	class scoped_spin_locker
	{
	public:
		scoped_spin_locker(spinlock& lock): lock_(lock) { lock_.lock(); }
		~scoped_spin_locker() { lock_.unlock(); }
	private:
		scoped_spin_locker(scoped_spin_locker const&);
		scoped_spin_locker& operator = (scoped_spin_locker const&);
		spinlock& lock_;
	};
}


#endif		// SALVIAR_ATOMIC_H
