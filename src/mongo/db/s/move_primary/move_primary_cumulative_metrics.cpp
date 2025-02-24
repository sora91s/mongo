/**
 *    Copyright (C) 2023-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#include "mongo/db/s/move_primary/move_primary_cumulative_metrics.h"

namespace mongo {
namespace {

constexpr auto kMovePrimary = "movePrimary";

const auto kReportedStateFieldNamesMap = [] {
    return MovePrimaryCumulativeMetrics::StateFieldNameMap{
        {MovePrimaryDonorState::kInitializing, "countInstancesInDonorState1Initializing"},
        {MovePrimaryDonorState::kCloning, "countInstancesInDonorState2Cloning"},
        {MovePrimaryDonorState::kWaitingToBlockWrites,
         "countInstancesInDonorState3WaitingToBlockWrites"},
        {MovePrimaryDonorState::kBlockingWrites, "countInstancesInDonorState4BlockingWrites"},
        {MovePrimaryDonorState::kPrepared, "countInstancesInDonorState5Prepared"},
        {MovePrimaryDonorState::kAborted, "countInstancesInDonorState6Aborted"},
        {MovePrimaryRecipientState::kStarted, "countInstancesInRecipientState1Initializing"},
        {MovePrimaryRecipientState::kConsistent, "countInstancesInRecipientState2Consistent"},
        {MovePrimaryRecipientState::kBlocking, "countInstancesInRecipientState3Blocking"},
        {MovePrimaryRecipientState::kAborted, "countInstancesInRecipientState4Aborted"},
        {MovePrimaryRecipientState::kDone, "countInstancesInRecipientState5Done"}};
}();

}  // namespace

MovePrimaryCumulativeMetrics::MovePrimaryCumulativeMetrics()
    : move_primary_cumulative_metrics::Base(
          kMovePrimary, std::make_unique<MovePrimaryCumulativeMetricsFieldNameProvider>()),
      _fieldNames(
          static_cast<const MovePrimaryCumulativeMetricsFieldNameProvider*>(getFieldNames())) {}

void MovePrimaryCumulativeMetrics::reportActive(BSONObjBuilder* bob) const {
    ShardingDataTransformCumulativeMetrics::reportActive(bob);
    reportOplogApplicationCountMetrics(_fieldNames, bob);
}

void MovePrimaryCumulativeMetrics::reportLatencies(BSONObjBuilder* bob) const {
    ShardingDataTransformCumulativeMetrics::reportLatencies(bob);
    reportOplogApplicationLatencyMetrics(_fieldNames, bob);
}

void MovePrimaryCumulativeMetrics::reportCurrentInSteps(BSONObjBuilder* bob) const {
    ShardingDataTransformCumulativeMetrics::reportCurrentInSteps(bob);
    reportCountsForAllStates(kReportedStateFieldNamesMap, bob);
}
}  // namespace mongo
