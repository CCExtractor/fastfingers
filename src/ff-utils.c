#include "ff-utils.h"

typedef struct dynamicArray {
    void *array;
    size_t inUse;
    size_t itemSize;
    size_t arraySize;
} dynamicArray;

void ff_init_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkScreen *screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(
            screen, GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_css_provider_load_from_resource(
            provider, "/org/ccextractor/FastFingers/FastFingers.css");

    g_object_unref(provider);
}

cJSON *ff_find_shortcut_by_id(cJSON *app, int id){
    cJSON *group = cJSON_GetObjectItemCaseSensitive(app, "group");
    for (int i = 0; i < cJSON_GetArraySize(group); ++i) {
        cJSON *category = cJSON_GetArrayItem(group, i);
        cJSON *shortcuts = cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
        for (int j = 0; j < cJSON_GetArraySize(shortcuts); ++j) {
            cJSON *shortcut = cJSON_GetArrayItem(shortcuts, j);
            if (cJSON_GetObjectItemCaseSensitive(shortcut, "id")->valueint == id)
                return shortcut;
        }
    }
    return NULL;
}

char *ff_simplify_title(const char *title) {
    char *simplified = malloc(sizeof(title));
    if (!simplified) {
        ff_error(" - Error: Couldn't allocate memory space for "
                 "simplified string!\n");
        return NULL;
    }

    char *cptr = simplified;
    while (*title) {
        if (*title != ' ')
            *cptr++ = tolower(*title);
        ++title;
    }
    *cptr = 0;

    return simplified;
}

char *ff_logo_path_gen(const char *title) {
    char *logo_path = malloc(sizeof(char) * 128);
    if (!logo_path) {
        ff_error(" - Error: Couldn't allocate memory space for "
                 "logo_path string!\n");
        return NULL;
    }

    sprintf(logo_path, "/org/ccextractor/FastFingers/logo/");
    char *cptr = logo_path + strlen(logo_path);
    while (*title) {
        if (*title != ' ')
            *cptr++ = tolower(*title);
        ++title;
    }
    sprintf(cptr, ".png");

    return logo_path;
}

struct _resizable_container {
    GtkWidget *image;
    GdkPixbuf *pixbuf;
};

cJSON *ff_read_json(const char *path) {
    FILE *file = NULL;
    cJSON *app = NULL;
    char *data = NULL;
    unsigned long len, result;

    file = fopen(path, "rb");
    if (!file) {
        ff_error("Couldn't open file %s: %s\n", path,
                 strerror(errno));

        return NULL;
    }

    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);

    data = (char *) malloc(len + 1);
    if (!data) {
        ff_error("Couldn't allocate memory space!\n");
        goto out;
    }

    result = fread(data, 1, len, file);
    if (result != len) {
        ff_error("Reading error occurred!\n");
        goto out;
    }

    data[len] = '\0';

    app = cJSON_Parse(data);

    if (!app) {
        ff_error("Error before: [%s]\n", cJSON_GetErrorPtr());
        goto out;
    }

    out:
    if (file)
        fclose(file);
    if (data)
        free(data);

    return app;
}

void ff_write_JSON_to_file(cJSON *json, const char *path) {
    char *out = cJSON_Print(json);

    FILE *fp = NULL;
    fp = fopen(path, "w");

    if (!fp) {
        ff_error("Couldn't open file %s: %s\n", path,
                 strerror(errno));
        goto end;
    }

    int written = fprintf(fp, "%s", out);

    if (written < 0) {
        ff_error("Couldn't write to file %s: %s\n", path,
                 strerror(errno));
        goto end;
    }

    end:
    if (fp)
        fclose(fp);
    free(out);
}

void ff_prepare_appdata(void) {
    const char *homedir = ff_get_home_dir();
    char path[128];
    struct stat st = {0};

    sprintf(path, "%s/.fastfingers", homedir);
    if (stat(path, &st) == -1) {
        mkdir(path, 0777);
    }

    sprintf(path, "%s/.fastfingers/applications", homedir);
    if (stat(path, &st) == -1) {
        mkdir(path, 0777);
    }

    cJSON *appdata = ff_get_application("appdata");
    if (!appdata) {
        appdata = cJSON_CreateObject();
        cJSON_AddArrayToObject(appdata, "recent");

        sprintf(path, "%s/.fastfingers/applications/appdata.json", homedir);
        ff_write_JSON_to_file(appdata, path);
    }

    DIR *d;
    struct dirent *dir;
    sprintf(path, "%s/.fastfingers/model", homedir);
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d))) {
            if (!g_str_has_suffix(dir->d_name, ".json"))
                continue;
            if (!strcmp(dir->d_name, "appdata.json"))
                continue;
            char *name = g_strndup(dir->d_name, strlen(dir->d_name) - 5);
            sprintf(path, "%s/.fastfingers/applications/%s.json", homedir, name);
            if (stat(path, &st) == -1) {
                ff_fetch_application_data(name);
            }
        }
    }
}

void ff_fetch_application_data(const char *name) {
    cJSON *model;
    char usr_share_path[128];
    char home_path[128];
    char data_path[128];
    const char *homedir = ff_get_home_dir();

    sprintf(usr_share_path, "/usr/share/fastfingers/applications/%s.json", name);
    model = ff_read_json(usr_share_path);

    if (!model) {
        sprintf(home_path, "%s/.fastfingers/model/%s.json", homedir, name);
        model = ff_read_json(home_path);
    }

    if (!model) {
        ff_error("No application file found %s neither in path '%s' or '%s'\n", name, usr_share_path,
                 home_path);
        return;
    }

    int categoryID = 1;
    int shortcutID = 1;
    cJSON_AddArrayToObject(model, "recent");
    cJSON *group = cJSON_GetObjectItemCaseSensitive(model, "group");
    for (int i = 0; i < cJSON_GetArraySize(group); ++i) {
        cJSON *category = cJSON_GetArrayItem(group, i);
        cJSON_AddNumberToObject(category, "id", categoryID++);
        cJSON *categoryShortcutArray = cJSON_GetObjectItemCaseSensitive(category, "shortcuts");
        for (int j = 0; j < cJSON_GetArraySize(categoryShortcutArray); ++j) {
            cJSON *shortcut = cJSON_GetArrayItem(categoryShortcutArray, j);
            cJSON_AddNumberToObject(shortcut, "id", shortcutID++);
            cJSON_AddNumberToObject(shortcut, "learned", 0);
        }
    }

    sprintf(data_path, "%s/.fastfingers/applications/%s.json", homedir, name);
    ff_write_JSON_to_file(model, data_path);
}

cJSON *ff_get_application(const char *name) {
    char path[128];
    const char *homedir = ff_get_home_dir();

    sprintf(path, "%s/.fastfingers/applications/%s.json", homedir, name);

    cJSON *app = NULL;
    app = ff_read_json(path);

    if (!app) {
        ff_error("Couldn't get the application %s, trying to fetch.\n", name);
        ff_fetch_application_data(name);
        app = ff_read_json(path);
        if (!app) {
            ff_error("Couldn't fetch the application %s.\n", name);
            return NULL;
        }
    }

    return app;
}

void set_scaled_image(GtkImage *image, const char *title, int size) {
    int raw_width, raw_height, new_width, new_height;
    char *logo_path;
    GdkPixbuf *raw_pixbuf, *scaled_pixbuf;

    logo_path = ff_logo_path_gen(title);

    if (!logo_path)
        return;

    raw_pixbuf = gdk_pixbuf_new_from_resource(logo_path, NULL);

    raw_width = gdk_pixbuf_get_width(raw_pixbuf);
    raw_height = gdk_pixbuf_get_height(raw_pixbuf);

    if (raw_width > raw_height) {
        new_width = size;
        new_height = raw_height * ((double) new_width / raw_width);
    } else {
        new_height = size;
        new_width = raw_width * ((double) new_height / raw_height);
    }

    scaled_pixbuf = gdk_pixbuf_scale_simple(raw_pixbuf, new_width, new_height,
                                            GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(image, scaled_pixbuf);

    free(logo_path);
}

void right_to_left(guint *keyval) {
    if (*keyval == gdk_keyval_from_name("Control_R"))
        *keyval = gdk_keyval_from_name("Control_L");
    if (*keyval == gdk_keyval_from_name("Alt_R"))
        *keyval = gdk_keyval_from_name("Alt_L");
    if (*keyval == gdk_keyval_from_name("Shift_R"))
        *keyval = gdk_keyval_from_name("Shift_L");
    if (*keyval == gdk_keyval_from_name("Super_R"))
        *keyval = gdk_keyval_from_name("Super_L");
}

void normalize_keyval(guint *keyval) {
    if (!strcmp("ISO_Left_Tab", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("Tab");
    if (!strcmp("KP_Add", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("plus");
    if (!strcmp("KP_Divide", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("slash");
    if (!strcmp("KP_Multiply", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("asterisk");
    if (!strcmp("KP_Subtract", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("minus");
    if (!strcmp("KP_Enter", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("Return");
    if (!strcmp("KP_0", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("0");
    if (!strcmp("KP_1", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("1");
    if (!strcmp("KP_2", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("2");
    if (!strcmp("KP_3", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("3");
    if (!strcmp("KP_4", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("4");
    if (!strcmp("KP_5", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("5");
    if (!strcmp("KP_6", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("6");
    if (!strcmp("KP_7", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("7");
    if (!strcmp("KP_8", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("8");
    if (!strcmp("KP_9", gdk_keyval_name(*keyval)))
        *keyval = gdk_keyval_from_name("9");
}

int key_compare(guint keyval1, guint keyval2) {
    right_to_left(&keyval1);
    right_to_left(&keyval2);
    normalize_keyval(&keyval1);
    normalize_keyval(&keyval2);
    if (gdk_keyval_to_upper(keyval1) == gdk_keyval_to_upper(keyval2))
        return 1;

    return 0;
}

char *get_keyval_name(guint keyval) {
    normalize_keyval(&keyval);
    return normalize_keyval_name(gdk_keyval_name(keyval));
}

guint get_keyval_from_name(const char *str) {
    if (!strcmp("Control", str))
        return gdk_keyval_from_name("Control_L");
    if (!strcmp("Alt", str))
        return gdk_keyval_from_name("Alt_L");
    if (!strcmp("Shift", str))
        return gdk_keyval_from_name("Shift_L");
    if (!strcmp("Space", str))
        return gdk_keyval_from_name("space");
    if (!strcmp("?", str))
        return gdk_keyval_from_name("exclam");
    if (!strcmp("\"", str))
        return gdk_keyval_from_name("quotedbl");
    if (!strcmp("$", str))
        return gdk_keyval_from_name("dollar");
    if (!strcmp("%", str))
        return gdk_keyval_from_name("percent");
    if (!strcmp("&", str))
        return gdk_keyval_from_name("ampersand");
    if (!strcmp("'", str))
        return gdk_keyval_from_name("apostrophe");
    if (!strcmp("'", str))
        return gdk_keyval_from_name("quoteright");
    if (!strcmp("(", str))
        return gdk_keyval_from_name("parenleft");
    if (!strcmp(")", str))
        return gdk_keyval_from_name("parenright");
    if (!strcmp("*", str))
        return gdk_keyval_from_name("asterisk");
    if (!strcmp("+", str))
        return gdk_keyval_from_name("plus");
    if (!strcmp(",", str))
        return gdk_keyval_from_name("comma");
    if (!strcmp("-", str))
        return gdk_keyval_from_name("minus");
    if (!strcmp(".", str))
        return gdk_keyval_from_name("period");
    if (!strcmp("/", str))
        return gdk_keyval_from_name("slash");
    if (!strcmp("\\", str))
        return gdk_keyval_from_name("backslash");

    return gdk_keyval_from_name(str);
}

char *normalize_keyval_name(const char *str) {
    size_t len = strlen(str);

    char *ret = NULL;

    if (len == 1)
        ret = g_strdup_printf("%c", toupper(*str));

    else if (!strcmp(str, "Control") || !strcmp(str, "Control_L") ||
             !strcmp(str, "Control_R"))
        ret = g_strdup("Ctrl");

    else if (!strcmp(str, "Alt") || !strcmp(str, "Alt_L") ||
             !strcmp(str, "Alt_R"))
        ret = g_strdup("Alt");

    else if (!strcmp(str, "Shift") || !strcmp(str, "Shift_L") ||
             !strcmp(str, "Shift_R"))
        ret = g_strdup("⇧");

    else if (!strcmp(str, "BackSpace"))
        ret = g_strdup("⌫");

    else if (!strcmp(str, "Return"))
        ret = g_strdup("⏎");

    else if (!strcmp(str, "Delete"))
        ret = g_strdup("Del");

    else if (!strcmp(str, "Right"))
        ret = g_strdup("→");

    else if (!strcmp(str, "Left"))
        ret = g_strdup("←");

    else if (!strcmp(str, "Up"))
        ret = g_strdup("↑");

    else if (!strcmp(str, "Down"))
        ret = g_strdup("↓");

    else if (!strcmp(str, "space"))
        ret = g_strdup("␣");

    else if (!strcmp(str, "Escape"))
        ret = g_strdup("Esc");

    else if (!strcmp(str, "Caps_Lock"))
        ret = g_strdup("⇪");

    else if (!strcmp(str, "quotedbl"))
        ret = g_strdup("\"");

    else if (!strcmp(str, "asterisk"))
        ret = g_strdup("*");

    else if (!strcmp(str, "minus"))
        ret = g_strdup("-");

    else if (!strcmp(str, "less"))
        ret = g_strdup("<");

    else if (!strcmp(str, "greater"))
        ret = g_strdup(">");

    else if (!strcmp(str, "period"))
        ret = g_strdup(".");

    else if (!strcmp(str, "comma"))
        ret = g_strdup(",");

    else if (!strcmp(str, "exclam"))
        ret = g_strdup("?");

    else if (!strcmp(str, "dollar"))
        ret = g_strdup("$");

    else if (!strcmp(str, "percent"))
        ret = g_strdup("%");

    else if (!strcmp(str, "ampersand"))
        ret = g_strdup("&");

    else if (!strcmp(str, "apostrophe") || !strcmp(str, "quoteright"))
        ret = g_strdup("'");

    else if (!strcmp(str, "parenleft"))
        ret = g_strdup("(");

    else if (!strcmp(str, "parenright"))
        ret = g_strdup(")");

    else if (!strcmp(str, "plus"))
        ret = g_strdup("+");

    else if (!strcmp(str, "slash"))
        ret = g_strdup("/");

    else if (!strcmp(str, "backslash"))
        ret = g_strdup("\\");

    else if (!strcmp(str, "ISO_Level3_Shift"))
        ret = g_strdup("AltGr");

    else if (!strcmp(str, "Super_L") || !strcmp(str, "Super_R"))
        ret = g_strdup("Super");

    else if (!strcmp(str, "Insert"))
        ret = g_strdup("Ins");

    else if (!strcmp(str, "Page_Up"))
        ret = g_strdup("Pg Up");

    else if (!strcmp(str, "Page_Down"))
        ret = g_strdup("Pg Dn");

    else if (!strcmp(str, "Scroll_Lock"))
        ret = g_strdup("⤓");

    else if (!strcmp(str, "Num_Lock"))
        ret = g_strdup("⇭");

    else
        ret = g_strdup(str);

    return ret;
}

void null_check(void *check_ptr, const char *str, void (*fptr)(void *),
                void *free_arg) {
    if (check_ptr)
        return;

    if (fptr && free_arg)
        ff_error("%s is null!\n", str);

    fptr(free_arg);
}

GtkWidget *ff_box_nth_child(GtkWidget *box, int idx) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(box));
    GtkWidget *ret = g_list_nth(children, idx)->data;
    g_list_free(children);

    return ret;
}

GtkWidget *ff_box_first_child(GtkWidget *box) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(box));
    GtkWidget *ret = g_list_first(children)->data;
    g_list_free(children);

    return ret;
}

GtkWidget *ff_box_last_child(GtkWidget *box) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(box));
    GtkWidget *ret = g_list_last(children)->data;
    g_list_free(children);

    return ret;
}

void add_style_class(GtkWidget *widget, const char *class) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(context, class);
}

void remove_style_class(GtkWidget *widget, const char *class) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_remove_class(context, class);
}

static void remove_all_cb(GtkWidget *widget, GtkContainer *container) {
    gtk_container_remove(container, widget);
}

void ff_container_remove_all(GtkWidget *container) {
    if (!GTK_IS_CONTAINER(container)) {
        ff_error("%p is not a container to remove all children\n",
                 container);
    }
    gtk_container_foreach(GTK_CONTAINER(container), GTK_CALLBACK(remove_all_cb),
                          GTK_CONTAINER(container));
}

dynamicArray *ff_dynamicArray_new(size_t itemSize) {

    dynamicArray *arr = malloc(sizeof(dynamicArray));
    if (!arr) {
        ff_error("Couldn't allocate enough space for dynamic array context!");
        return NULL;
    }

    arr->inUse = 0;
    arr->arraySize = 1;
    arr->itemSize = itemSize;

    arr->array = malloc(arr->arraySize * itemSize);
    if (!arr->array) {
        ff_error("Couldn't allocate enough space for dynamic array!");
        return NULL;
    }

    return arr;
}

size_t ff_dynamicArray_append(dynamicArray *arr, const void *item) {
    if (arr->arraySize == arr->inUse) {
        void *tmp = realloc(arr->array, arr->arraySize * arr->itemSize * 2);
        if (!tmp) {
            ff_error("Couldn't expand dynamic array's size!");
            return 0;
        }
        arr->array = tmp;
    }

    memcpy(arr->array + arr->inUse * arr->itemSize, item, arr->itemSize);
    return ++arr->inUse;
}

size_t ff_dynamicArray_prepend(dynamicArray *arr, const void *item) {
    if (arr->arraySize == arr->inUse) {
        void *tmp = realloc(arr->array, arr->arraySize * arr->itemSize * 2);
        if (!tmp) {
            ff_error("Couldn't expand dynamic array's size!");
            return 0;
        }
        arr->array = tmp;
    }

    memmove(arr->array + arr->itemSize, arr->array, arr->itemSize);
    memcpy(arr->array, item, arr->itemSize);

    return ++arr->inUse;
}

void *ff_dynamicArray_get(dynamicArray *arr, size_t idx) {
    return arr->array + arr->itemSize * idx;
}

size_t ff_dynamicArray_size(dynamicArray *arr) {
    return arr->inUse;
}

void ff_dynamicArray_free(dynamicArray *arr) {
    free(arr->array);
    free(arr);
}

const char *ff_get_home_dir(void) {
    const char *homedir = NULL;
    homedir = getenv("HOME");
    if (!homedir) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    return homedir;
}