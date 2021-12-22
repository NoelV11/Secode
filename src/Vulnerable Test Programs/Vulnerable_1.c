#include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>
#include "ignite/ignite.h"
#include "ignite/ignition.h"

#include "ignite/examples/person.h"
#include "ignite/examples/organization.h"

using namespace ignite;

using namespace examples;

    int64_t keyId = 0;
        int64_t keyOrgIdAff = 0;

  int keyId = 1;
    int    keyOrgIdAff = 1;

        orgId = keyOrgIdAff;
        strncpy(firstName, "John", sizeof(firstName));  //vulnerable function strncpy x 3 instances
        strncpy(lastName, "Doe", sizeof(lastName));      //Secode will flag these instances and mention it in the summary .csv
        strncpy(resume, "Master Degree.", sizeof(resume));
        salary = 2200.0;

        ret = SQLExecute(stmt);

        if (!SQL_SUCCEEDED(ret))
            ThrowOdbcError(SQL_HANDLE_STMT, stmt, "Failed to execute prepared statement");

        ret = SQLMoreResults(stmt);

        if (ret != SQL_NO_DATA)
            ThrowOdbcError(SQL_HANDLE_STMT, stmt, "No more data expected");

return 0;
}
