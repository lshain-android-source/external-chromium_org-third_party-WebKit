/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Portions are Copyright (C) 2002 Netscape Communications Corporation.
 * Other contributors: David Baron <dbaron@fas.harvard.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Alternatively, the document type parsing portions of this file may be used
 * under the terms of either the Mozilla Public License Version 1.1, found at
 * http://www.mozilla.org/MPL/ (the "MPL") or the GNU General Public
 * License Version 2.0, found at http://www.fsf.org/copyleft/gpl.html
 * (the "GPL"), in which case the provisions of the MPL or the GPL are
 * applicable instead of those above.  If you wish to allow use of your
 * version of this file only under the terms of one of those two
 * licenses (the MPL or the GPL) and not to allow others to use your
 * version of this file under the LGPL, indicate your decision by
 * deleting the provisions above and replace them with the notice and
 * other provisions required by the MPL or the GPL, as the case may be.
 * If you do not delete the provisions above, a recipient may use your
 * version of this file under any of the LGPL, the MPL or the GPL.
 */

#include "config.h"
#include "core/html/HTMLDocument.h"

#include "HTMLNames.h"
#include "bindings/v8/ScriptController.h"
#include "core/html/HTMLBodyElement.h"
#include "core/page/FocusController.h"
#include "core/page/Frame.h"
#include "core/page/FrameTree.h"
#include "core/page/FrameView.h"
#include "core/page/Page.h"

namespace WebCore {

using namespace HTMLNames;

HTMLDocument::HTMLDocument(const DocumentInit& initializer, DocumentClassFlags extendedDocumentClasses)
    : Document(initializer, HTMLDocumentClass | extendedDocumentClasses)
{
    ScriptWrappable::init(this);
    clearXMLVersion();
}

HTMLDocument::~HTMLDocument()
{
}

int HTMLDocument::width()
{
    updateLayoutIgnorePendingStylesheets();
    FrameView* frameView = view();
    return frameView ? frameView->contentsWidth() : 0;
}

int HTMLDocument::height()
{
    updateLayoutIgnorePendingStylesheets();
    FrameView* frameView = view();
    return frameView ? frameView->contentsHeight() : 0;
}

String HTMLDocument::dir()
{
    HTMLElement* b = body();
    if (!b)
        return String();
    return b->getAttribute(dirAttr);
}

void HTMLDocument::setDir(const String& value)
{
    HTMLElement* b = body();
    if (b)
        b->setAttribute(dirAttr, value);
}

String HTMLDocument::designMode() const
{
    return inDesignMode() ? "on" : "off";
}

void HTMLDocument::setDesignMode(const String& value)
{
    InheritedBool mode;
    if (equalIgnoringCase(value, "on"))
        mode = on;
    else if (equalIgnoringCase(value, "off"))
        mode = off;
    else
        mode = inherit;
    Document::setDesignMode(mode);
}

Element* HTMLDocument::activeElement()
{
    if (Element* element = treeScope()->adjustedFocusedElement())
        return element;
    return body();
}

bool HTMLDocument::hasFocus()
{
    Page* page = this->page();
    if (!page)
        return false;
    if (!page->focusController().isActive() || !page->focusController().isFocused())
        return false;
    if (Frame* focusedFrame = page->focusController().focusedFrame()) {
        if (focusedFrame->tree()->isDescendantOf(frame()))
            return true;
    }
    return false;
}

inline HTMLBodyElement* HTMLDocument::bodyAsHTMLBodyElement() const
{
    HTMLElement* element = body();
    return (element && element->hasTagName(bodyTag)) ? toHTMLBodyElement(element) : 0;
}

String HTMLDocument::bgColor()
{
    if (HTMLBodyElement* bodyElement = bodyAsHTMLBodyElement())
        return bodyElement->bgColor();
    return String();
}

void HTMLDocument::setBgColor(const String& value)
{
    if (HTMLBodyElement* bodyElement = bodyAsHTMLBodyElement())
        bodyElement->setBgColor(value);
}

String HTMLDocument::fgColor()
{
    if (HTMLBodyElement* bodyElement = bodyAsHTMLBodyElement())
        return bodyElement->text();
    return String();
}

void HTMLDocument::setFgColor(const String& value)
{
    if (HTMLBodyElement* bodyElement = bodyAsHTMLBodyElement())
        bodyElement->setText(value);
}

String HTMLDocument::alinkColor()
{
    if (HTMLBodyElement* bodyElement = bodyAsHTMLBodyElement())
        return bodyElement->aLink();
    return String();
}

void HTMLDocument::setAlinkColor(const String& value)
{
    if (HTMLBodyElement* bodyElement = bodyAsHTMLBodyElement()) {
        // This check is a bit silly, but some benchmarks like to set the
        // document's link colors over and over to the same value and we
        // don't want to incur a style update each time.
        if (bodyElement->aLink() != value)
            bodyElement->setALink(value);
    }
}

String HTMLDocument::linkColor()
{
    if (HTMLBodyElement* bodyElement = bodyAsHTMLBodyElement())
        return bodyElement->link();
    return String();
}

void HTMLDocument::setLinkColor(const String& value)
{
    if (HTMLBodyElement* bodyElement = bodyAsHTMLBodyElement()) {
        // This check is a bit silly, but some benchmarks like to set the
        // document's link colors over and over to the same value and we
        // don't want to incur a style update each time.
        if (bodyElement->link() != value)
            bodyElement->setLink(value);
    }
}

String HTMLDocument::vlinkColor()
{
    if (HTMLBodyElement* bodyElement = bodyAsHTMLBodyElement())
        return bodyElement->vLink();
    return String();
}

void HTMLDocument::setVlinkColor(const String& value)
{
    if (HTMLBodyElement* bodyElement = bodyAsHTMLBodyElement()) {
        // This check is a bit silly, but some benchmarks like to set the
        // document's link colors over and over to the same value and we
        // don't want to incur a style update each time.
        if (bodyElement->vLink() != value)
            bodyElement->setVLink(value);
    }
}

// --------------------------------------------------------------------------
// not part of the DOM
// --------------------------------------------------------------------------

void HTMLDocument::addItemToMap(HashCountedSet<StringImpl*>& map, const AtomicString& name)
{
    if (name.isEmpty())
        return;
    map.add(name.impl());
    if (Frame* f = frame())
        f->script()->namedItemAdded(this, name);
}

void HTMLDocument::removeItemFromMap(HashCountedSet<StringImpl*>& map, const AtomicString& name)
{
    if (name.isEmpty())
        return;
    map.remove(name.impl());
    if (Frame* f = frame())
        f->script()->namedItemRemoved(this, name);
}

void HTMLDocument::addNamedItem(const AtomicString& name)
{
    addItemToMap(m_namedItemCounts, name);
}

void HTMLDocument::removeNamedItem(const AtomicString& name)
{
    removeItemFromMap(m_namedItemCounts, name);
}

void HTMLDocument::addExtraNamedItem(const AtomicString& name)
{
    addItemToMap(m_extraNamedItemCounts, name);
}

void HTMLDocument::removeExtraNamedItem(const AtomicString& name)
{
    removeItemFromMap(m_extraNamedItemCounts, name);
}

static void addLocalNameToSet(HashSet<StringImpl*>* set, const QualifiedName& qName)
{
    set->add(qName.localName().impl());
}

static HashSet<StringImpl*>* createHtmlCaseInsensitiveAttributesSet()
{
    // This is the list of attributes in HTML 4.01 with values marked as "[CI]" or case-insensitive
    // Mozilla treats all other values as case-sensitive, thus so do we.
    HashSet<StringImpl*>* attrSet = new HashSet<StringImpl*>;

    addLocalNameToSet(attrSet, accept_charsetAttr);
    addLocalNameToSet(attrSet, acceptAttr);
    addLocalNameToSet(attrSet, alignAttr);
    addLocalNameToSet(attrSet, alinkAttr);
    addLocalNameToSet(attrSet, axisAttr);
    addLocalNameToSet(attrSet, bgcolorAttr);
    addLocalNameToSet(attrSet, charsetAttr);
    addLocalNameToSet(attrSet, checkedAttr);
    addLocalNameToSet(attrSet, clearAttr);
    addLocalNameToSet(attrSet, codetypeAttr);
    addLocalNameToSet(attrSet, colorAttr);
    addLocalNameToSet(attrSet, compactAttr);
    addLocalNameToSet(attrSet, declareAttr);
    addLocalNameToSet(attrSet, deferAttr);
    addLocalNameToSet(attrSet, dirAttr);
    addLocalNameToSet(attrSet, disabledAttr);
    addLocalNameToSet(attrSet, enctypeAttr);
    addLocalNameToSet(attrSet, faceAttr);
    addLocalNameToSet(attrSet, frameAttr);
    addLocalNameToSet(attrSet, hreflangAttr);
    addLocalNameToSet(attrSet, http_equivAttr);
    addLocalNameToSet(attrSet, langAttr);
    addLocalNameToSet(attrSet, languageAttr);
    addLocalNameToSet(attrSet, linkAttr);
    addLocalNameToSet(attrSet, mediaAttr);
    addLocalNameToSet(attrSet, methodAttr);
    addLocalNameToSet(attrSet, multipleAttr);
    addLocalNameToSet(attrSet, nohrefAttr);
    addLocalNameToSet(attrSet, noresizeAttr);
    addLocalNameToSet(attrSet, noshadeAttr);
    addLocalNameToSet(attrSet, nowrapAttr);
    addLocalNameToSet(attrSet, readonlyAttr);
    addLocalNameToSet(attrSet, relAttr);
    addLocalNameToSet(attrSet, revAttr);
    addLocalNameToSet(attrSet, rulesAttr);
    addLocalNameToSet(attrSet, scopeAttr);
    addLocalNameToSet(attrSet, scrollingAttr);
    addLocalNameToSet(attrSet, selectedAttr);
    addLocalNameToSet(attrSet, shapeAttr);
    addLocalNameToSet(attrSet, targetAttr);
    addLocalNameToSet(attrSet, textAttr);
    addLocalNameToSet(attrSet, typeAttr);
    addLocalNameToSet(attrSet, valignAttr);
    addLocalNameToSet(attrSet, valuetypeAttr);
    addLocalNameToSet(attrSet, vlinkAttr);

    return attrSet;
}

bool HTMLDocument::isCaseSensitiveAttribute(const QualifiedName& attributeName)
{
    static HashSet<StringImpl*>* htmlCaseInsensitiveAttributesSet = createHtmlCaseInsensitiveAttributesSet();
    bool isPossibleHTMLAttr = !attributeName.hasPrefix() && (attributeName.namespaceURI() == nullAtom);
    return !isPossibleHTMLAttr || !htmlCaseInsensitiveAttributesSet->contains(attributeName.localName().impl());
}

void HTMLDocument::clear()
{
    // FIXME: This does nothing, and that seems unlikely to be correct.
    // We've long had a comment saying that IE doesn't support this.
    // But I do see it in the documentation for Mozilla.
}

}
