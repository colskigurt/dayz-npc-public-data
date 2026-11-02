## 1. 작업 목표

정상 낙하·충돌·착지·추적 재개를 보존하면서 기존6m 예산 소진 뒤 착지 복구 제한 불일치를 국소 수정한다. 성능·애니메이션과 별도로 기능을 검증한다.

## 2. 실제 수행

실제 개체를 움직이지 않는 별도 body에 입력·접촉·포즈 접근자를 연결해 기존 PressureEscapeStepClear를0.02m 단위 최대60단계 사용한다. 실제 지면·ground영역·높이차0.35m 이하를 매 단계 확인한다. 착지와 전체 포즈 검사가3단계 연속 통과해야 기존0.04m 정지 허용오차를 포함한 경로로 인정한다.

원래 PlanPressureEscape가 선택한 방향만 검사하며 추가 거리 권한은 검사된 길이+0.04m, 최대1.2m다. 기존6m 누계는 초기화하지 않는다. 한 낙하 generation에서 한 번 부여하고 검사 길이/0.6m/s+0.5초(상한2.5초)로 만료한다. 전투·방향 변화·지지 변경·실제 접촉 거부 시 취소한다. 실제 이동은 기존 PressureSlide를 재사용하여 모든 월드 검사·접촉·lift·낙하 이동 집계를 유지한다. 기존 lease와 중첩 부여하지 않는다.

먼저 고정 입력·취소 조건을 격리 서버에서 시험한 뒤 별도 PBO를 빌드·서명·멤버 검증했고, 실제 PBO 로딩 상태로 동일 시험을 다시 통과했다. 기존300초 activeReference 임무·카메라·SupportMode off·terrain/escape refinement on 조건으로8→64 각각1회 실행했다.

## 3. 측정 결과

|개체 수|도착|낙하 후 복귀|상태 전이 동일 호출 대조|mission dt p50/p95/p99|전경 비율|
|---|---|---|---|---|---|
|8|8/8|9/9|112/112|20.0/33.0/34.0ms|1.0|
|64|64/64|78/78|916/916|26.0/56.0/77.0ms|1.0|

위 프레임 값은Present/CPU self-time/표시 애니메이션 간격이 아니다. OFF/ON 반복 성능 판정에 사용하지 않는다.

고정 입력2건에서0.6m 탐색은 거부,1m 탐색은0.839844m·43단계의 검사 경로를 확인했다. 권한 부여 후 두 입력은 약0.809/0.797m 실제 논리 이동에서 착지·전체 포즈 검사 통과 지점에 도달했다. 기존 누계·예산과 실제 오브젝트 좌표를 보존했다. 접촉·방향·전투·거리 취소8조건과 만료2조건이 통과했다. 이는 실제 연속 군집의 전체 재현이 아니다.

실제 후보 이벤트 집계:
```json
{
  "checked-vertical8": {},
  "checked-vertical64": {}
}
```

## 4. 원인 분석

확인 사실: 원래 목표 선택은1m였고 후속 정책은 목표0.75m·추가거리0.6m로 제한되어 기존 실패 입력의 유효0.80m 경로를 이어받지 못했다. 새 후보는 검사 결과를 부여 조건에 연결한다. 낙하나 지지를 강제 성공시키지 않았다.

시험 도중 취소 함수의 한 줄 조기return 뒤 대입 형태에서 취소 플래그가false로 남았지만 이동 반환false·좌표변화0·상태EXPIRED였다. 같은 함수의 명시적 블록/줄 분리만 변경한 뒤 플래그와 조건이 통과했다. 보존된 c/d 원문을 대조했으며 엔진 컴파일러의 구체적 원인은 미확정이다. 앞선 컴파일 실패는 시험 코드 삽입 위치 오류로 별도 보존했다.

현재64 단일 실행 기능 통과 여부: True. 실제 새 복구 권한 GRANTED 관찰: False. 권한이 사용되지 않은 실행은 정상 경로 회귀 근거이며 새 복구의 실제 효능을 입증하지 않는다. 통과하더라도 동일 조건 반복·OFF 비교가 없어 원인별 개선률이나 전체 일반 맵 안정성을 확정하지 않는다. fallback은 추가 질의를 발생시키므로 최적화 성공으로 보고하지 않는다.

## 5. 검증 수준

고정 입력·취소 조건: 진단 성공. 실제8/64: 위 표의 단일 회귀 결과만 인정. 반복 검증 완료/최종 수용 아님. 시각 품질과 성능 개선은 미측정이며20ms p95는 목표로 유지한다. 전체 골은 미완료다.

## 6. 변경 사항

격리 NFCheckedLandingPlan.c·NFCheckedLandingPolicy.c·설정 플래그·실행 프로필·분석만 추가했다. 후보 PBO SHA-256 1F8FB4DD930BAF99C92C35B76EE2461A9D38B7315BC38731EBE38166A1D81B9D. 서명 및 패키지 내 소스 일치 검증 성공. 실제 서버/클라이언트 컴파일·실행 결과를 연결했다. 원본 게임 소스·기존 PBO·운영 서버·Steam 배포본·애니메이션 자산은 변경하지 않았고 개인키는 배포/공개하지 않았다.

## 7. 원시 증거

실행ID: checked-plan, checked-policy(컴파일 실패), checked-policyb/c(취소 플래그 실패), checked-policyd/e(통과), checked-addon(실제 PBO 통과), checked-vertical8/64. 같은 입력의 재시험은 독립적인 군집 반복으로 세지 않는다. raw TSV·RPT·launch·result·원문을 로컬 체크포인트 SHA-256 목록에 연결했다. 공개 패킷은 선택한 자체 후보 함수와 집계만 제공하며 원시 대형 기록·로컬 경로·프로세스 정보·게임 자산은 노출하지 않는다.

## 8. 잔여 문제

If no live grant occurred, use a bounded existing-fixture full-NPC activation test before adoption; blind64 repeats without exercising the policy are lower value. If an activated recovery fails, inspect actual cancellation/pose/contact inputs before changing limits. Matched OFF/ON and repeated64 acceptance remain pending; animation and scratch performance stay separate.

새 fallback의 계획 횟수·실제 부여/취소·단계 검사 수와 기능 결과를 비교해야 한다. 단일 favorable run으로 개선을 선언하지 않는다. 애니메이션 상위 누적기·8파트 표시 문제와 scratch 배열 성능 후보는 별개로 유지한다. 추가 임의 보간·갱신 생략·충돌 검사 삭제는 적용하지 않는다.

로컬 checkpoint SHA-256: f9cb927abfec31ec14ab9c66bd5369b959c8b2c6742443a8b2739897d076e526
