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

#pragma once

#include "mongo/db/s/cumulative_metrics_state_holder.h"
#include "mongo/db/s/metrics/sharding_data_transform_cumulative_metrics.h"
#include "mongo/db/s/metrics/sharding_data_transform_metrics_macros.h"
#include "mongo/db/s/metrics/with_oplog_application_count_metrics.h"
#include "mongo/db/s/metrics/with_oplog_application_latency_metrics.h"
#include "mongo/db/s/metrics/with_state_management_for_cumulative_metrics.h"
#include "mongo/db/s/move_primary/move_primary_cumulative_metrics_field_name_provider.h"
#include "mongo/db/s/move_primary/move_primary_state_machine_gen.h"

namespace mongo {

namespace move_primary_cumulative_metrics {
DEFINE_IDL_ENUM_SIZE_TEMPLATE_HELPER(MovePrimaryMetrics,
                                     MovePrimaryDonorState,
                                     MovePrimaryRecipientState)
using Base = WithOplogApplicationLatencyMetrics<WithOplogApplicationCountMetrics<
    WithStateManagementForCumulativeMetrics<ShardingDataTransformCumulativeMetrics,
                                            MovePrimaryMetricsEnumSizeTemplateHelper,
                                            MovePrimaryDonorState,
                                            MovePrimaryRecipientState>>>;
}  // namespace move_primary_cumulative_metrics

class MovePrimaryCumulativeMetrics : public move_primary_cumulative_metrics::Base {
public:
    MovePrimaryCumulativeMetrics();

private:
    virtual void reportActive(BSONObjBuilder* bob) const;
    virtual void reportLatencies(BSONObjBuilder* bob) const;
    virtual void reportCurrentInSteps(BSONObjBuilder* bob) const;

    const MovePrimaryCumulativeMetricsFieldNameProvider* _fieldNames;
};

}  // namespace mongo
