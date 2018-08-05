﻿// Copyright (c) 2015-2016, Tammo Beil - All rights reserved

#pragma once

#include "Components/Widget.h"
#include "BRSSpinBoxInt.generated.h"




struct FUnitTextNumericType : TDefaultNumericTypeInterface<int32>
{
	FUnitTextNumericType(FText* UnitPtr)
		: UnitText(UnitPtr)
	{}

	FText* UnitText;


	/** Convert the type to/from a string */
	virtual FString ToString(const int32& Value) const override
	{
		FString ValueString = TDefaultNumericTypeInterface<int32>::ToString(Value);
		return FString::Printf(TEXT("%s %s"), *ValueString, (UnitText ? *(UnitText->ToString()) : TEXT("")));
	}
	virtual TOptional<int32> FromString(const FString& InString, const int32& InExistingValue) override
	{
		FString ValueString = InString;
		if (UnitText)
		{
			ValueString.RemoveFromEnd(UnitText->ToString());
		}
		int32 NewValue;
		bool bEvalResult = LexTryParseString(NewValue, *ValueString);
		if (!bEvalResult)
		{
			float FloatValue = 0.f;
			bEvalResult = FMath::Eval(*ValueString, FloatValue);
			NewValue = FloatValue;
		}

		return bEvalResult ? NewValue : InExistingValue; //TOptional<int32>();
	}
};



/**
* A numerical entry box that allows for direct entry of the number or allows the user to click and slide the number.
*/
UCLASS()
class BRSENGINE_API UBRSSpinBoxInt : public UWidget
{
	GENERATED_UCLASS_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpinBoxIntValueChangedEvent, int32, InValue);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpinBoxIntValueCommittedEvent, int32, InValue, ETextCommit::Type, CommitMethod);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpinBoxBeginSliderMovement);

public:

	PROPERTY_BINDING_IMPLEMENTATION(int32, Value);
	/** Value stored in this spin box */
	UPROPERTY(EditAnywhere, Category = Content)
		int32 Value;

	/** A bindable delegate to allow logic to drive the value of the widget */
	UPROPERTY()
		FGetInt32 ValueDelegate;

public:
	/** The Style */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "Style"))
		FSpinBoxStyle WidgetStyle;

	UPROPERTY()
		USlateWidgetStyleAsset* Style_DEPRECATED;

	/** The amount by which to change the spin box value as the slider moves. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider")
		int32 Delta;

	/** The exponent by which to increase the delta as the mouse moves. 1 is constant (never increases the delta). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider")
		float SliderExponent;

	/** Font color and opacity (overrides style) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
		FSlateFontInfo Font;

	/** The minimum width of the spin box */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display", AdvancedDisplay, DisplayName = "Minimum Desired Width")
		float MinDesiredWidth;

	/** Whether to remove the keyboard focus from the spin box when the value is committed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", AdvancedDisplay)
		bool ClearKeyboardFocusOnCommit;

	/** Whether to select the text in the spin box when the value is committed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", AdvancedDisplay)
		bool SelectAllTextOnCommit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
		FSlateColor ForegroundColor;

public:
	/** Called when the value is changed interactively by the user */
	UPROPERTY(BlueprintAssignable, Category = "SpinBox|Events")
		FOnSpinBoxIntValueChangedEvent OnValueChanged;

	/** Called when the value is committed. Occurs when the user presses Enter or the text box loses focus. */
	UPROPERTY(BlueprintAssignable, Category = "SpinBox|Events")
		FOnSpinBoxIntValueCommittedEvent OnValueCommitted;

	/** Called right before the slider begins to move */
	UPROPERTY(BlueprintAssignable, Category = "SpinBox|Events")
		FOnSpinBoxBeginSliderMovement OnBeginSliderMovement;

	/** Called right after the slider handle is released by the user */
	UPROPERTY(BlueprintAssignable, Category = "SpinBox|Events")
		FOnSpinBoxIntValueChangedEvent OnEndSliderMovement;

public:

	/** Get the current value of the spin box. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		int32 GetValue() const;

	/** Set the value of the spin box. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		void SetValue(int32 NewValue);

public:

	/** Get the current minimum value that can be manually set in the spin box. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		int32 GetMinValue() const;

	/** Set the minimum value that can be manually set in the spin box. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		void SetMinValue(int32 NewValue);

	/** Clear the minimum value that can be manually set in the spin box. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		void ClearMinValue();

	/** Get the current maximum value that can be manually set in the spin box. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		int32 GetMaxValue() const;

	/** Set the maximum value that can be manually set in the spin box. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		void SetMaxValue(int32 NewValue);

	/** Clear the maximum value that can be manually set in the spin box. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		void ClearMaxValue();

	/** Get the current minimum value that can be specified using the slider. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		int32 GetMinSliderValue() const;

	/** Set the minimum value that can be specified using the slider. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		void SetMinSliderValue(int32 NewValue);

	/** Clear the minimum value that can be specified using the slider. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		void ClearMinSliderValue();

	/** Get the current maximum value that can be specified using the slider. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		int32 GetMaxSliderValue() const;

	/** Set the maximum value that can be specified using the slider. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		void SetMaxSliderValue(int32 NewValue);

	/** Clear the maximum value that can be specified using the slider. */
	UFUNCTION(BlueprintCallable, Category = "Behavior")
		void ClearMaxSliderValue();

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetForegroundColor(FSlateColor InForegroundColor);

	UFUNCTION(BlueprintCallable, Category = "Unit")
		void SetUnitText(const FText& InUnitText) { UnitText = InUnitText; }

public:

	// UWidget interface
	virtual void SynchronizeProperties() override;
	// End of UWidget interface

	// UVisual interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UVisual interface

	// Begin UObject interface
	virtual void PostLoad() override;
	// End of UObject interface

#if WITH_EDITOR
//	virtual const FSlateBrush* GetEditorIcon() override;
	virtual const FText GetPaletteCategory() override;
#endif
	// End of UWidget interface

protected:

	TSharedPtr<FUnitTextNumericType> TypeInterface;

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget

	void HandleOnValueChanged(int32 InValue);
	void HandleOnValueCommitted(int32 InValue, ETextCommit::Type CommitMethod);
	void HandleOnBeginSliderMovement();
	void HandleOnEndSliderMovement(int32 InValue);

protected:

	/** Whether the optional MinValue attribute of the widget is set */
	UPROPERTY()
		uint32 bOverride_MinValue : 1;

	/** Whether the optional MaxValue attribute of the widget is set */
	UPROPERTY()
		uint32 bOverride_MaxValue : 1;

	/** Whether the optional MinSliderValue attribute of the widget is set */
	UPROPERTY()
		uint32 bOverride_MinSliderValue : 1;

	/** Whether the optional MaxSliderValue attribute of the widget is set */
	UPROPERTY()
		uint32 bOverride_MaxSliderValue : 1;

	/** The minimum allowable value that can be manually entered into the spin box */
	UPROPERTY(EditAnywhere, Category = Content, DisplayName = "Minimum Value", meta = (editcondition = "bOverride_MinValue"))
		int32 MinValue;

	/** The maximum allowable value that can be manually entered into the spin box */
	UPROPERTY(EditAnywhere, Category = Content, DisplayName = "Maximum Value", meta = (editcondition = "bOverride_MaxValue"))
		int32 MaxValue;

	/** The minimum allowable value that can be specified using the slider */
	UPROPERTY(EditAnywhere, Category = Content, DisplayName = "Minimum Slider Value", meta = (editcondition = "bOverride_MinSliderValue"))
		int32 MinSliderValue;

	/** The maximum allowable value that can be specified using the slider */
	UPROPERTY(EditAnywhere, Category = Content, DisplayName = "Maximum Slider Value", meta = (editcondition = "bOverride_MaxSliderValue"))
		int32 MaxSliderValue;

	/** The maximum allowable value that can be specified using the slider */
	UPROPERTY(EditAnywhere, Category = Content, DisplayName = "Unit Text", meta = (editcondition = "bOverride_MaxSliderValue"))
		FText UnitText;

protected:
	TSharedPtr<SSpinBox<int32>> MySpinBox;
};