/* Copyright 2017 Intel Corporation

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
------------------------------------------------------------------------------*/
#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "sawtooth_sdk.h"
#include "sawtooth/message_stream.h"

namespace sawtooth {

// Provides access to the global state
class GlobalStateImpl: public GlobalState {
 public:
    // Takes the messages stream that it will use to communicate with the
    // Validator to retrieve the current state. It is not expected that this
    // will be called directly from a Transaction Processor. An instance will
    // be provided to transaction handlers when a transaction is being
    // processed.
    explicit GlobalStateImpl(const MessageStreamPtr& message_stream,
        const std::string& context_id, ::google::protobuf::uint64 tip);
    virtual ~GlobalStateImpl() {}

    // Retrieve a single value from global state. If you are retrieving
    // multiple values, it is encouraged to use the batch get overload defined
    // below. The function returns false if the value is not retrieved, true if
    // it is found in the GlobalStore, and throws an exception if an error occurs
    bool GetState(std::string* out_value, const std::string& address) const;

    // Retrieve multiple values from the global state. Values are returned in
    // the out_values map. If a value is not present then there will be no
    // entry in the returned map. So you must check for the presence of a
    // returned value prior to accessing it.
    void GetState(std::unordered_map<std::string, std::string>* out_values,
        const std::vector<std::string>& addresses) const;

    // Set a single GlobalState value. Use the batch Set function defined
    // below if you are setting multiple items.
    void SetState(const std::string& address, const std::string& value) const;
    // Set multiple values in the global state. Each entry in the addresses
    // param is a std::pair, with the first value representing the address and
    // the second the value.
    //      std::vector<GlobalState::KeyValue> addresses;
    //      addresses.push(std::make_pair(address, value));
    void SetState(const std::vector<KeyValue>& addresses) const;

    // Delete a single GlobalState value. Used the batch Delete function
    // defined below if you are deleting multiple items.
    void DeleteState(const std::string& address) const;

    // Delete multiple entries from global state.
    void DeleteState(const std::vector<std::string>& address) const;

    // Add Events for every transaction
    void AddEvent(const std::string& event_type ,
       const std::vector<KeyValue>& kv_pairs, const std::string& event_data) const;

    virtual ::google::protobuf::uint64 GetTip() const;

    virtual void GetStatesByPrefix(const std::string& address, std::string* root, std::string* start, std::vector<KeyValue>* out_values) const;
    virtual void GetSigByNum(::google::protobuf::uint64 num, std::string* sig_out) const;
    virtual void GetBlockById(const std::string& block_id, BlockInfo* header_out) const;
    virtual void GetRewardBlockSignatures(const std::string& block_id, std::vector<std::string> &signatures, ::google::protobuf::uint64 first_pred, ::google::protobuf::uint64 last_pred) const;

private:
    ::google::protobuf::uint64 tip;
    std::string context_id;
    MessageStreamPtr message_stream;
    std::string block_signature;
};

}  // namespace sawtooth
