// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: challenge_rpc.proto
#ifndef GRPC_challenge_5frpc_2eproto__INCLUDED
#define GRPC_challenge_5frpc_2eproto__INCLUDED

#include "challenge_rpc.pb.h"

#include <functional>
#include <grpc/impl/codegen/port_platform.h>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

class Encrypt1 final {
 public:
  static constexpr char const* service_full_name() {
    return "Encrypt1";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::google::protobuf::BytesValue* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>> AsyncEncrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>>(AsyncEncryptRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>> PrepareAsyncEncrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>>(PrepareAsyncEncryptRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      virtual void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>* AsyncEncryptRaw(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>* PrepareAsyncEncryptRaw(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::google::protobuf::BytesValue* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>> AsyncEncrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>>(AsyncEncryptRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>> PrepareAsyncEncrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>>(PrepareAsyncEncryptRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, std::function<void(::grpc::Status)>) override;
      void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>* AsyncEncryptRaw(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>* PrepareAsyncEncryptRaw(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Encrypt_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Encrypt(::grpc::ServerContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Encrypt() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestEncrypt(::grpc::ServerContext* context, ::google::protobuf::BytesValue* request, ::grpc::ServerAsyncResponseWriter< ::google::protobuf::BytesValue>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Encrypt<Service > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_Encrypt() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::google::protobuf::BytesValue, ::google::protobuf::BytesValue>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response) { return this->Encrypt(context, request, response); }));}
    void SetMessageAllocatorFor_Encrypt(
        ::grpc::experimental::MessageAllocator< ::google::protobuf::BytesValue, ::google::protobuf::BytesValue>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc_impl::internal::CallbackUnaryHandler< ::google::protobuf::BytesValue, ::google::protobuf::BytesValue>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* Encrypt(
      ::grpc::CallbackServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* Encrypt(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_Encrypt<Service > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_Encrypt<Service > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Encrypt() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Encrypt() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestEncrypt(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_Encrypt() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Encrypt(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* Encrypt(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* Encrypt(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Encrypt() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::google::protobuf::BytesValue, ::google::protobuf::BytesValue>(
            [this](::grpc_impl::ServerContext* context,
                   ::grpc_impl::ServerUnaryStreamer<
                     ::google::protobuf::BytesValue, ::google::protobuf::BytesValue>* streamer) {
                       return this->StreamedEncrypt(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedEncrypt(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::google::protobuf::BytesValue,::google::protobuf::BytesValue>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_Encrypt<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_Encrypt<Service > StreamedService;
};

class Encrypt2 final {
 public:
  static constexpr char const* service_full_name() {
    return "Encrypt2";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::google::protobuf::BytesValue* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>> AsyncEncrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>>(AsyncEncryptRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>> PrepareAsyncEncrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>>(PrepareAsyncEncryptRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      virtual void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, std::function<void(::grpc::Status)>) = 0;
      virtual void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>* AsyncEncryptRaw(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::google::protobuf::BytesValue>* PrepareAsyncEncryptRaw(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::google::protobuf::BytesValue* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>> AsyncEncrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>>(AsyncEncryptRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>> PrepareAsyncEncrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>>(PrepareAsyncEncryptRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, std::function<void(::grpc::Status)>) override;
      void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void Encrypt(::grpc::ClientContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void Encrypt(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::google::protobuf::BytesValue* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>* AsyncEncryptRaw(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::google::protobuf::BytesValue>* PrepareAsyncEncryptRaw(::grpc::ClientContext* context, const ::google::protobuf::BytesValue& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Encrypt_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Encrypt(::grpc::ServerContext* context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Encrypt() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestEncrypt(::grpc::ServerContext* context, ::google::protobuf::BytesValue* request, ::grpc::ServerAsyncResponseWriter< ::google::protobuf::BytesValue>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Encrypt<Service > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_Encrypt() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::google::protobuf::BytesValue, ::google::protobuf::BytesValue>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::google::protobuf::BytesValue* request, ::google::protobuf::BytesValue* response) { return this->Encrypt(context, request, response); }));}
    void SetMessageAllocatorFor_Encrypt(
        ::grpc::experimental::MessageAllocator< ::google::protobuf::BytesValue, ::google::protobuf::BytesValue>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc_impl::internal::CallbackUnaryHandler< ::google::protobuf::BytesValue, ::google::protobuf::BytesValue>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* Encrypt(
      ::grpc::CallbackServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* Encrypt(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_Encrypt<Service > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_Encrypt<Service > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Encrypt() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Encrypt() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestEncrypt(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_Encrypt() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc_impl::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Encrypt(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* Encrypt(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* Encrypt(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Encrypt : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Encrypt() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::google::protobuf::BytesValue, ::google::protobuf::BytesValue>(
            [this](::grpc_impl::ServerContext* context,
                   ::grpc_impl::ServerUnaryStreamer<
                     ::google::protobuf::BytesValue, ::google::protobuf::BytesValue>* streamer) {
                       return this->StreamedEncrypt(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Encrypt() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Encrypt(::grpc::ServerContext* /*context*/, const ::google::protobuf::BytesValue* /*request*/, ::google::protobuf::BytesValue* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedEncrypt(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::google::protobuf::BytesValue,::google::protobuf::BytesValue>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_Encrypt<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_Encrypt<Service > StreamedService;
};


#endif  // GRPC_challenge_5frpc_2eproto__INCLUDED