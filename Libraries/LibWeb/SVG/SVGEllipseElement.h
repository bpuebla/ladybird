/*
 * Copyright (c) 2022, Sam Atkins <atkinssj@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibWeb/SVG/SVGAnimatedLength.h>
#include <LibWeb/SVG/SVGGeometryElement.h>

namespace Web::SVG {

class SVGEllipseElement final : public SVGGeometryElement {
    WEB_PLATFORM_OBJECT(SVGEllipseElement, SVGGeometryElement);
    GC_DECLARE_ALLOCATOR(SVGEllipseElement);

public:
    virtual ~SVGEllipseElement() override = default;

    virtual void attribute_changed(FlyString const& name, Optional<String> const& old_value, Optional<String> const& value, Optional<FlyString> const& namespace_) override;

    virtual Gfx::Path get_path(CSSPixelSize viewport_size) override;

    GC::Ref<SVGAnimatedLength> cx() const;
    GC::Ref<SVGAnimatedLength> cy() const;
    GC::Ref<SVGAnimatedLength> rx() const;
    GC::Ref<SVGAnimatedLength> ry() const;

private:
    SVGEllipseElement(DOM::Document&, DOM::QualifiedName);

    virtual void initialize(JS::Realm&) override;

    Optional<float> m_center_x;
    Optional<float> m_center_y;
    Optional<float> m_radius_x;
    Optional<float> m_radius_y;
};

}
