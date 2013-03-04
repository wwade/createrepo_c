/* createrepo_c - Library of routines for manipulation with repodata
 * Copyright (C) 2012  Tomas Mlcoch
 * Copyright (C) 2006  Seth Vidal
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include <string.h>
#include "package.h"
#include "logging.h"

#define PACKAGE_CHUNK_SIZE 2048

cr_Dependency *
cr_dependency_new(void)
{
    cr_Dependency *dep;

    dep = g_new0 (cr_Dependency, 1);

    return dep;
}

cr_PackageFile *
cr_package_file_new(void)
{
    cr_PackageFile *file;

    file = g_new0 (cr_PackageFile, 1);

    return file;
}

cr_ChangelogEntry *
cr_changelog_entry_new(void)
{
    cr_ChangelogEntry *entry;

    entry = g_new0 (cr_ChangelogEntry, 1);

    return entry;
}

cr_Package *
cr_package_new(void)
{
    cr_Package *package;

    package = g_new0 (cr_Package, 1);
    package->chunk = g_string_chunk_new (PACKAGE_CHUNK_SIZE);

    return package;
}

cr_Package *
cr_package_new_without_chunk(void)
{
    return g_new0(cr_Package, 1);
}

void
cr_package_free(cr_Package *package)
{
    if (!package)
        return;

    if (package->chunk)
        g_string_chunk_free (package->chunk);

/* Note: Since glib 2.28
 * g_slist_foreach && g_slist_free could be replaced with one function:
 * g_slist_free_full()
 */

    if (package->requires) {
        g_slist_foreach (package->requires, (GFunc) g_free, NULL);
        g_slist_free (package->requires);
    }

    if (package->provides) {
        g_slist_foreach (package->provides, (GFunc) g_free, NULL);
        g_slist_free (package->provides);
    }

    if (package->conflicts) {
        g_slist_foreach (package->conflicts, (GFunc) g_free, NULL);
        g_slist_free (package->conflicts);
    }

    if (package->obsoletes) {
        g_slist_foreach (package->obsoletes, (GFunc) g_free, NULL);
        g_slist_free (package->obsoletes);
    }

    if (package->files) {
        g_slist_foreach (package->files, (GFunc) g_free, NULL);
        g_slist_free (package->files);
    }

    if (package->changelogs) {
        g_slist_foreach (package->changelogs, (GFunc) g_free, NULL);
        g_slist_free (package->changelogs);
    }

    g_free (package);
}

gchar *
cr_package_nvra(cr_Package *package)
{
    return g_strdup_printf("%s-%s-%s.%s", package->name, package->version,
                                          package->release, package->arch);
}

gchar *
cr_package_nevra(cr_Package *package)
{
    char *epoch;
    if (package->epoch && strlen(package->epoch))
        epoch = package->epoch;
    else
        epoch = "0";

    return g_strdup_printf("%s-%s:%s-%s.%s", package->name, epoch,
                           package->version, package->release, package->arch);
}
