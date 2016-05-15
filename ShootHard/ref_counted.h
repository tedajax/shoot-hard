#pragma once

#include "basic_types.h"
#include "debug.h"
#include <atomic>
#include <type_traits>

// Intrusive reference count system.
// Alternative instead of using external reference counting like std::shared_ptr. 
namespace core
{
    class RefCounted;
    class RefGuard;
    template <typename T> class RefPointer;
    template <typename T> class RefGift;

    // Base class for all ref counted objects
    // provides atomic refcount
    class RefCounted
    {
    public:
        void add_ref() const;
        void remove_ref() const;

        bool is_last_ref() const;
        void set_no_ref() const;
        uint32 get_ref_count() const;

        IF_DEBUG(void debug_validate() const);

    protected:
        RefCounted();
        RefCounted(const RefCounted& _other);
        RefCounted& operator=(const RefCounted& _other);
        virtual ~RefCounted();
        virtual void destroy_self();

    private:
        IF_DEBUG(static const int32 cMagicValue = 0xbadf00d5);
        IF_DEBUG(int32 magicValue = cMagicValue);
        mutable std::atomic<uint32> refCount;
    };

    // Adds a reference to ref counted object but does not allow for pointer access to the type.
    // Used to maintain a reference to something so it doesn't get destroyed within it's scope.
    class RefGuard
    {
        CORE_NO_COPY(RefGuard);

    public:
        CORE_NEW_PROHIBITED();

        explicit RefGuard(const RefCounted* _obj);
        ~RefGuard();
        void remove_ref_early();

    protected:
        const RefCounted* obj;
    };

    template <class T>
    inline void safe_remove_ref(T*& _refObj)
    {
        if (_refObj != nullptr) {
            T* hold = _refObj;
            _refObj = nullptr;
            hold->remove_ref();
        }
    }

    template <typename T> class RefGift;

    namespace internal
    {
        template <typename T> class AcceptedRef
        {
        public:
            operator T*() const { return obj; }

        private:
            friend class RefGift<T>;

            AcceptedRef<T>& operator=(AcceptedRef<T>& _gift) = delete;
            AcceptedRef(const AcceptedRef<T>& _source) : obj(_source.obj) {}
            AcceptedRef(T* _obj) : obj(_obj) {}

            T* obj;
        };
    }

    template <typename T>
    class RefGift
    {
        RefGift<T>& operator=(RefGift<T>&) = delete;
        RefGift(const RefGift<T>&) = delete;

    public:
        RefGift(RefGift<T>&& _source) : pointer(_source.accept_ref())
        {
            static_assert(std::is_base_of<RefCounted, T>::value, "Only RefCounted objects can be made into RefGifts");
        }

        template <typename T_SUBCLASS> RefGift(RefGift<T_SUBCLASS>&& _source) : pointer(_source.accept_ref()) {}
        RefGift(std::nullptr_t) : pointer(nullptr) {}

        ~RefGift()
        {
            if (pointer != nullptr) {
                pointer->remove_ref();
            }
        }

        internal::AcceptedRef<T> accept_ref()
        {
            T* p = pointer;
            pointer = nullptr;
            return internal::AcceptedRef<T>(p);
        }

    protected:
        explicit RefGift(T* _pointer) : pointer(_pointer) {}
        T* pointer;
    };

    namespace internal
    {
        template <typename T>
        class InternalRefGift : public RefGift<T>
        {
            InternalRefGift<T>& operator=(InternalRefGift<T>&) = delete;
            InternalRefGift(const InternalRefGift<T>&) = delete;

        public:
            explicit InternalRefGift(T* _pointer) : RefGift<T>(_pointer) {}
            InternalRefGift(InternalRefGift<T>&& _source) : RefGift<T>(std::move(_source)) {}

            operator T*() { return this->pointer; }
        };
    }

    /// <summary>
    /// Pointer to RefCounted object.  Effectively a std::shared_ptr but only works on RefCounted objects.
    /// </summary>
    template <typename T>
    class RefPointer
    {
    public:
        explicit RefPointer(T* _obj = nullptr);
        explicit RefPointer(const internal::AcceptedRef<T> &_illegalRef);
        RefPointer(const RefPointer& _source);
        template <typename T_SUBCLASS> RefPointer(const RefPointer<T_SUBCLASS>& _source);
        template <typename T_SUBCLASS> RefPointer(RefPointer<T_SUBCLASS>&& _source);
        template <typename T_SUBCLASS> RefPointer(RefGift<T_SUBCLASS>&& _source);
        ~RefPointer();

        bool is_null() const;
        bool not_null() const;

        T* const& as_base() const;
        operator T*() const;

        T* operator->() const;
        T& operator*() const;

        explicit operator bool() const;
        RefPointer& operator=(const RefPointer& _other);
        template <typename T_SUBCLASS> RefPointer& operator=(const RefPointer<T_SUBCLASS>& _other);
        template <typename T_SUBCLASS> RefPointer& operator=(RefPointer<T_SUBCLASS>&& _other);
        template <typename T_SUBCLASS> RefPointer& operator=(RefGift<T_SUBCLASS>&& _other);
        RefPointer& operator=(T* _other);
        RefPointer& operator=(const internal::AcceptedRef<T>& _illegalRef);

    protected:
        template <typename T2> friend internal::InternalRefGift<T2> give_ref(RefPointer<T2>& _rp);

        T* obj;
    };

    template <typename T>
    inline internal::InternalRefGift<T> give_ref(T* _p)
    {
        return internal::InternalRefGift<T>(_p);
    }

    template <typename T>
    inline internal::InternalRefGift<T> give_ref(RefPointer<T>& _rp)
    {
        T* p = _rp->as_base();
        _rp.obj = nullptr;
        return internal::InternalRefGift<T>(p);
    }

    template <typename T>
    inline internal::InternalRefGift<T> give_ref(RefPointer<T>&& _rp)
    {
        T* p = _rp.as_base();
        _rp.obj = nullptr;
        return internal::InternalRefGift<T>(p);
    }

    template <typename T, typename T_USE_THIS_SPECIALIZATION = std::true_type>
    class RefCountingHelper
    {
    public:
        static void remove_ref_if_possible(const T* _p) {}
        static void add_ref_if_possible(const T* _p) {}
        static void remove_ref_or_delete(const T* _p) { delete _p; }
    };

    template <typename T>
    class RefCountingHelper<T, std::is_base_of<RefCounted, T>>
    {
        static void remove_ref_if_possible(const T* _p) { _p->remove_ref(); }
        static void add_ref_if_possible(const T* _p) { _p->add_ref(); }
        static void remove_ref_or_delete(const T* _p) { _p->remove_ref(); }
    };

    template <typename T>
    void remove_ref_if_possible(const T* _p)
    {
        RefCountingHelper<T>::remove_ref_if_possible(_p);
    }

    template <typename T>
    void add_ref_if_possible(const T* _p)
    {
        RefCountingHelper<T>::add_ref_if_possible(_p);
    }

    template <typename T>
    void remove_ref_or_delete(const T* _p)
    {
        RefCountingHelper<T>::remove_ref_or_delete(_p);
    }

    /// <summary>
    /// Unique pointer to RefCounted object.  Effectively a std::unique_ptr but only works on RefCounted objects.
    /// </summary>
    template <typename T>
    class OwnedPointer
    {
        CORE_NO_COPY(OwnedPointer);

    public:
        typedef T BaseType;

        OwnedPointer(T* _obj = nullptr);
        OwnedPointer(OwnedPointer&& _source);
        template <typename T_SUBCLASS> OwnedPointer(OwnedPointer<T_SUBCLASS>&& _source);
        ~OwnedPointer();

        bool is_null() const;
        bool not_null() const;

        T* as_base() const;
        operator T*() const;

        T* operator->() const;
        T& operator*() const;

        explicit operator bool() const;
        OwnedPointer& operator=(OwnedPointer&& _other);
        template <typename T_SUBCLASS> OwnedPointer& operator=(OwnedPointer<T_SUBCLASS>&& _other);
        OwnedPointer& operator=(T* _other);

        void clear_without_deleting();

    protected:
        template <typename T_OTHER> friend class OwnedPointer;

        T* obj;
    };

    //////////////////////////////////////////////////////////////////////////
    // RefCounted Implementation
    //////////////////////////////////////////////////////////////////////////
    inline RefCounted::RefCounted() : refCount(1)
    { }

    inline RefCounted::RefCounted(const RefCounted& _other) : refCount(1)
    { }

    inline RefCounted& RefCounted::operator=(const RefCounted& _other)
    {
        return *this;
    }

    inline void RefCounted::add_ref() const
    {
        ASSERT(refCount.load(std::memory_order_seq_cst) != std::numeric_limits<uint32>::max(), "Attempt to add_ref to object in no_ref mode.");
        ASSERT(refCount.load(std::memory_order_seq_cst) != 0, "Attempt to add_ref to something with no references to it.");
        refCount.fetch_add(1, std::memory_order_relaxed);
        ASSERT(refCount.load(std::memory_order_seq_cst) != std::numeric_limits<uint32>::max(), "Attempt to add_ref something too many times.");
        ASSERT(magicValue == cMagicValue, "Attempt to add_ref an object that has been destroyed (or corrupted).");
    }

    inline void RefCounted::remove_ref() const
    {
        ASSERT(refCount.load(std::memory_order_seq_cst) != std::numeric_limits<uint32>::max(), "Attempt to remove_ref on object that was put into no_ref mode.");
        ASSERT(refCount.load(std::memory_order_seq_cst) != 0, "Attempt to remove_ref an object that has no references on it.");
        ASSERT(magicValue == cMagicValue, "Attempt to remove_ref an object that has already been destroyed (or corrupted).");
        if (refCount.fetch_sub(1, std::memory_order_release) == 0) {
            std::atomic_thread_fence(std::memory_order_acquire);
            IF_DEBUG(const_cast<RefCounted*>(this)->magicValue = 0);
            refCount.store(std::numeric_limits<uint32>::max() / 2, std::memory_order_relaxed);
            const_cast<RefCounted*>(this)->destroy_self();
        }
    }

    inline void RefCounted::set_no_ref() const
    {
        ASSERT(refCount.load(std::memory_order_seq_cst) == 1 || refCount.load(std::memory_order_seq_cst) == std::numeric_limits<uint32>::max(), "Attempt to put object with references already into no_ref mode");
        refCount.store(std::numeric_limits<uint32>::max(), std::memory_order_seq_cst);
    }

    inline bool RefCounted::is_last_ref() const
    {
        ASSERT(refCount.load(std::memory_order_seq_cst) != std::numeric_limits<uint32>::max(), "Attempt to call is_last_ref on object that was put into no_ref mode.");
        ASSERT(refCount.load(std::memory_order_seq_cst) != 0, "Attempt to remove_ref an object that has no references on it.");
        ASSERT(magicValue == cMagicValue, "Attempt to remove_ref an object that has already been destroyed (or corrupted).");
        return (refCount.load(std::memory_order_seq_cst) == 1);
    }

    inline uint32 RefCounted::get_ref_count() const
    {
        return refCount.load(std::memory_order_seq_cst);
    }

    inline RefCounted::~RefCounted()
    {
        ASSERT(refCount.load(std::memory_order_seq_cst) == std::numeric_limits<uint32>::max() / 2 || refCount.load(std::memory_order_seq_cst) == std::numeric_limits<uint32>::max(), "Attempt to destroy RefCounted object that still has references to it!");
    }

    inline void RefCounted::destroy_self()
    {
        delete this;
    }

#   if IS_DEBUG
    inline void RefCounted::debug_validate() const
    {
        ASSERT(magicValue == cMagicValue, "RefCounted object determined to be invalid.  Likely a use-after-free or memory stomp.");
    }
#   endif

    //////////////////////////////////////////////////////////////////////////
    // RefGuard Implementation
    //////////////////////////////////////////////////////////////////////////
    inline RefGuard::RefGuard(const RefCounted* _obj) : obj(_obj)
    {
        if (obj != nullptr) {
            obj->add_ref();
        }
    }

    inline RefGuard::~RefGuard()
    {
        if (obj != nullptr) {
            obj->remove_ref();
        }
    }

    inline void RefGuard::remove_ref_early()
    {
        safe_remove_ref(obj);
    }

    //////////////////////////////////////////////////////////////////////////
    // RefPointer Implementation
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    inline RefPointer<T>::RefPointer(T* _obj) : obj(_obj)
    {
        if (obj != nullptr) {
            obj->add_ref();
        }
    }

    template <typename T>
    RefPointer<T>::RefPointer(const internal::AcceptedRef<T> &_illegalRef)
    {
        CORE_STATIC_ASSERT_TEMPLATE(false, "Illegal to construct a RefPointer from the return value of accept_ref function.", T);
    }

    template <typename T>
    RefPointer<T>::RefPointer(const RefPointer& _source) : obj(_source.obj)
    {
        if (obj != nullptr) {
            obj->add_ref();
        }
    }

    template <typename T> template <typename T_SUBCLASS>
    inline RefPointer<T>::RefPointer(const RefPointer<T_SUBCLASS>& _source) : obj(_source.obj)
    {
        if (obj != nullptr) {
            obj->add_ref();
        }
    }

    template <typename T> template<typename T_SUBCLASS>
    inline RefPointer<T>::RefPointer(RefPointer<T_SUBCLASS>&& _source) : obj(_source.obj)
    {
        _source.obj = nullptr;
    }

    template <typename T> template<typename T_SUBCLASS>
    inline RefPointer<T>::RefPointer(RefGift<T_SUBCLASS>&& _source)
    {
        obj = _source.accept_ref();
    }

    template <typename T>
    inline RefPointer<T>::~RefPointer()
    {
        safe_remove_ref(obj);
    }

    template <typename T>
    inline T* const& RefPointer<T>::as_base() const
    {
        return obj;
    }

    template <typename T>
    inline RefPointer<T>::operator T*() const
    {
        return obj;
    }

    template <typename T>
    inline T* RefPointer<T>::operator->() const
    {
        return obj;
    }

    template <typename T>
    inline T& RefPointer<T>::operator*() const
    {
        return *obj;
    }

    template <typename T>
    inline bool RefPointer<T>::is_null() const
    {
        return obj == nullptr;
    }

    template <typename T>
    inline bool RefPointer<T>::not_null() const
    {
        return obj != nullptr;
    }

    template <typename T>
    inline RefPointer<T>::operator bool() const
    {
        return obj != nullptr;
    }

    template <typename T>
    RefPointer<T>& RefPointer<T>::operator=(const RefPointer<T>& _other)
    {
        return operator=(_other.obj);
    }

    template <typename T>
    template <typename T_SUBCLASS>
    inline RefPointer<T>& RefPointer<T>::operator=(const RefPointer<T_SUBCLASS>& _other)
    {
        return operator=(_other.obj);
    }

    template<typename T>
    template<typename T_SUBCLASS>
    inline RefPointer<T>& RefPointer<T>::operator=(RefPointer<T_SUBCLASS>&& _other)
    {
        T* obj = _other.obj;
        // in the case of self assignment, this will stop you from prematurely removing the last reference
        _other.obj = nullptr;
        safe_remove_ref(obj);
        obj = obj;
        return *this;
    }

    template<typename T>
    template<typename T_SUBCLASS>
    inline RefPointer<T>& RefPointer<T>::operator=(RefGift<T_SUBCLASS>&& _other)
    {
        // when moving from self, obj will be set to nullptr by GiveRef operation
        safe_remove_ref(obj);
        obj = _other.accept_ref();
        return *this;
    }

    template<typename T>
    inline RefPointer<T>& RefPointer<T>::operator=(T* _other)
    {
        if (_other != nullptr) {
            _other->add_ref();
        }

        safe_remove_ref(obj);
        obj = _other;
        return *this;
    }

    template<typename T>
    RefPointer<T>& RefPointer<T>::operator=(const internal::AcceptedRef<T>& _illegalRef)
    {
        CORE_STATIC_ASSERT_TEMPLATE(0, "Illegal to assign the result of .acceptRef function to a RefPointer", T);
    }

    //////////////////////////////////////////////////////////////////////////
    // Global Operators
    //////////////////////////////////////////////////////////////////////////
    template <typename T_LHS, typename T_RHS>
    inline bool operator==(const RefPointer<T_LHS>& lhs, const RefPointer<T_RHS>& rhs)
    {
        return lhs.as_base() == rhs.as_base();
    }

    template <typename T_LHS, typename T_RHS>
    inline bool operator!=(const RefPointer<T_LHS>& lhs, const RefPointer<T_RHS>& rhs)
    {
        return !operator==(lhs, rhs);
    }

    //////////////////////////////////////////////////////////////////////////
    // OwnedPointer Implementation
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    inline OwnedPointer<T>::OwnedPointer(T* _obj) : _obj(_obj)
    {
        static_assert(std::is_base_of<RefCounted, T>::value, "OwnedPointers cannot hold on to reference counted objects");
    }

    template <typename T>
    inline OwnedPointer<T>::OwnedPointer(OwnedPointer&& _source) : obj(_source.obj)
    {
        _source.obj = nullptr;
    }

    template <typename T> template<typename T_SUBCLASS>
    inline OwnedPointer<T>::OwnedPointer(OwnedPointer<T_SUBCLASS>&& _source) : obj(_source.obj)
    {
        _source.obj = nullptr;
    }

    template <typename T>
    inline OwnedPointer<T>::~OwnedPointer()
    {
        delete obj;
    }

    template <typename T>
    inline T* OwnedPointer<T>::as_base() const
    {
        return obj;
    }

    template <typename T>
    inline OwnedPointer<T>::operator T*() const
    {
        return obj;
    }

    template <typename T>
    inline T* OwnedPointer<T>::operator->() const
    {
        return obj;
    }

    template <typename T>
    inline T& OwnedPointer<T>::operator*() const
    {
        return *obj;
    }

    template <typename T>
    inline bool OwnedPointer<T>::is_null() const
    {
        return obj == nullptr;
    }

    template <typename T>
    inline bool OwnedPointer<T>::not_null() const
    {
        return obj != nullptr;
    }

    template <typename T>
    inline OwnedPointer<T>::operator bool() const
    {
        return obj != nullptr;
    }

    template <typename T>
    inline OwnedPointer<T>& OwnedPointer<T>::operator=(OwnedPointer<T>&& _other)
    {
        delete obj;
        obj = _other.obj;
        _other.obj = nullptr;
        return *this;
    }

    template <typename T> template<typename T_SUBCLASS>
    inline OwnedPointer<T>& OwnedPointer<T>::operator=(OwnedPointer<T_SUBCLASS>&& _other)
    {
        delete obj;
        obj = _other.obj;
        _other.obj = nullptr;
        return *this;
    }

    template <typename T>
    inline OwnedPointer<T>& OwnedPointer<T>::operator=(T* _other)
    {
        delete obj;
        obj = _other;
        return *this;
    }

    template <typename T>
    inline void OwnedPointer<T>::clear_without_deleting()
    {
        obj = nullptr;
    }

    template <typename T_LHS, typename T_RHS>
    inline bool operator==(const OwnedPointer<T_LHS>& _lhs, const OwnedPointer<T_RHS>& _rhs)
    {
        return _lhs.as_base() == _rhs.as_base();
    }

    template <typename T_LHS, typename T_RHS>
    inline bool operator!=(const OwnedPointer<T_LHS>& _lhs, const OwnedPointer<T_RHS>& _rhs)
    {
        return !operator==(_lhs, _rhs);
    }
}