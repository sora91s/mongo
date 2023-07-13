//
// A DBQuery which is explained rather than executed normally. Also could be thought of as
// an "explainable cursor". Explains of .find() operations run through this abstraction.
//

var DBExplainQuery = (function() {
    //
    // Private methods.
    //

    /**
     * Many of the methods of an explain query just pass through to the underlying
     * non-explained DBQuery. Use this to generate a function which calls function 'name' on
     * 'destObj' and then returns this.
     */
    function createDelegationFunc(explainQuery, dbQuery, name) {
        return function() {
            dbQuery[name].apply(dbQuery, arguments);
            return explainQuery;
        };
    }

    function constructor(query, verbosity) {
        //
        // Private vars.
        //

        this._query = query;
        this._verbosity = Explainable.parseVerbosity(verbosity);
        this._mongo = query._mongo;
        this._finished = false;

        // Used if this query is a count, not a find.
        this._isCount = false;
        this._applySkipLimit = false;

        //
        // Public delegation methods. These just pass through to the underlying
        // DBQuery.
        //

        var delegationFuncNames = [
            "addOption",
            "allowDiskUse",
            "batchSize",
            "collation",
            "comment",
            "hint",
            "limit",
            "max",
            "maxTimeMS",
            "min",
            "readPref",
            "showDiskLoc",
            "skip",
            "sort",
        ];

        // Generate the delegation methods from the list of their names.
        var that = this;
        delegationFuncNames.forEach(function(name) {
            that[name] = createDelegationFunc(that, that._query, name);
        });

        //
        // Core public methods.
        //

        /**
         * Indicates that we are done building the query to explain, and sends the explain
         * command or query to the server.
         *
         * Returns the result of running the explain.
         */
        this.finish = function() {
            if (this._finished) {
                throw Error("query has already been explained");
            }

            // Mark this query as finished. Shouldn't be used for another explain.
            this._finished = true;

            // Explain always gets pretty printed.
            this._query._prettyShell = true;

            // Convert this explain query into an explain command, and send the command to
            // the server.
            var innerCmd;
            if (this._isCount) {
                // True means to always apply the skip and limit values.
                innerCmd = this._query._convertToCountCmd(this._applySkipLimit);
            } else {
                var canAttachReadPref = false;
                innerCmd = this._query._convertToCommand(canAttachReadPref);
            }

            var explainCmd = {explain: innerCmd};
            explainCmd["verbosity"] = this._verbosity;
            // If "maxTimeMS" is set on innerCmd, it needs to be propagated to the top-level
            // of explainCmd so that it has the intended effect.
            if (innerCmd.hasOwnProperty("maxTimeMS")) {
                explainCmd.maxTimeMS = innerCmd.maxTimeMS;
            }

            var explainDb = this._query._db;

            if ("$readPreference" in this._query._query) {
                var prefObj = this._query._query.$readPreference;
                explainCmd = explainDb._attachReadPreferenceToCommand(explainCmd, prefObj);
            }

            var explainResult = explainDb.runReadCommand(explainCmd, null, this._query._options);

            return Explainable.throwOrReturn(explainResult);
        };

        this.next = function() {
            return this.finish();
        };

        this.hasNext = function() {
            return !this._finished;
        };

        this.forEach = function(func) {
            while (this.hasNext()) {
                func(this.next());
            }
        };

        /**
         * Returns the explain resulting from running this query as a count operation.
         *
         * If 'applySkipLimit' is true, then the skip and limit values set on this query values are
         * passed to the server; otherwise they are ignored.
         */
        this.count = function(applySkipLimit) {
            this._isCount = true;
            if (applySkipLimit) {
                this._applySkipLimit = true;
            }
            return this.finish();
        };

        /**
         * This gets called automatically by the shell in interactive mode. It should
         * print the result of running the explain.
         */
        this.shellPrint = function() {
            var result = this.finish();
            return tojson(result);
        };

        /**
         * Display help text.
         */
        this.help = function() {
            var res = "";
            res += "Explain query methods\n";
            res += "\t.finish() - sends explain command to the server and returns the result\n";
            res += "\t.forEach(func) - apply a function to the explain results\n";
            res += "\t.hasNext() - whether this explain query still has a result to retrieve\n";
            res += "\t.next() - alias for .finish()\n";
            res += "Explain query modifiers\n";
            res += "\t.addOption(n)\n";
            res += "\t.batchSize(n)\n";
            res += "\t.comment(comment)\n";
            res += "\t.collation(collationSpec)\n";
            res += "\t.count()\n";
            res += "\t.hint(hintSpec)\n";
            res += "\t.limit(n)\n";
            res += "\t.maxTimeMS(n)\n";
            res += "\t.max(idxDoc)\n";
            res += "\t.min(idxDoc)\n";
            res += "\t.readPref(mode, tagSet)\n";
            res += "\t.showDiskLoc()\n";
            res += "\t.skip(n)\n";
            res += "\t.sort(sortSpec)\n";
            return res;
        };
    }

    return constructor;
})();
