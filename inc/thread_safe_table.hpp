#pragma once

#include <shared_mutex>
#include <unordered_map>

namespace otus {
  class ThreadSafeTable {
  public:
    using TableType = std::unordered_map<int, std::string>;

    TableType const & getSharedLocked() {
      mutex.lock_shared();
      return table;
    }

    void sharedUnlock() { mutex.unlock_shared(); }

    TableType & getLocked() {
      mutex.lock();
      return table;
    }

    void unlock() {
      mutex.unlock();
    }

  private:
    TableType table { };
    std::shared_mutex mutex { };
  };
}
