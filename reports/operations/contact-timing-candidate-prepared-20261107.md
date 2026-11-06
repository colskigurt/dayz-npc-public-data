# 접촉 내부 시간 계측 생략 후보: 준비 완료·실행 미검증

## 1. 작업 목표
새 물리 검사 축소보다 먼저 접촉 솔버에 항상 들어가는 상세 시간 계측의 실행 비용을 줄일 수 있는지 격리 후보를 준비한다. 재사용만이 아닌 불필요한 네이티브 시계 호출 자체의 제거 후보다. 물리 질의/밀침/낙하/착지/추적/재등반은 유지한다.

## 2. 실제 수행
현재5파트 서버 비용 보고서와 DriveAttribution 소스를 대조했다. DriveAttribution OFF는 외부 추가 계측을 끌 뿐 기존 솔버 내부 TickCount는 유지한다. 기반 SolveContacts/Internal 두 메서드를 읽고 시간 읽기·누적만 제거한 default-OFF 애드온을 생성했다. Fast/Shadow 특수 모드는 원본으로 fallback하고 SupportFast 등 솔버 override 애드온과는 조합하지 않는다. 기반 소스는 변경하지 않았다. 격리 PBO 빌드·서명·서명 검증, packed script 바이트 일치, 현재 대조군의 다른 애드온에 solver override가 없는지 확인했다.8개체 OFF/ON 계획2개와 복귀/수용 기준을 준비했다. 게임은 실행하지 않았다.

## 3. 측정 결과
기존 대조군15.737게임초의45,220solve/180,827관절sweep을 소스와 연결하면 정상 스코프의 TickCount 호출은 최소632,974회, 게임초당40,222.02회다. solve당 outer/bone/foot 각2회와 실행joint sweep당2회이며 추가 진단/fallback 시간 호출은 제외한다. 호출 수를 CPU 시간으로 환산하지 않는다. 새 CPU·Present p50/p95/p99·애니메이션 표시·기능 결과는 없다. 기존 첫쌍 물리 Fast 후보의 실패는 그대로 유지하고 이번 후보 효과로 전용하지 않는다.

## 4. 원인 분석
확인:현재 코어 소스에서 대상 시간 누적 필드는 진단용 쓰기이며 조회한 현재 시나리오/선택 애드온의 제어조건으로 쓰이지 않는다. 직접 물리 API 호출5개 식과 copied body의 비계측 연산은 정적 비교에서 일치했다. 추정:매우 빈번한 네이티브 시계 호출과 누적 연산을 없애면 전체CPU를 줄일 가능성이 있다. 미확인:시계 호출 자체 비용, 기본OFF wrapper 비용, 실제8/64 기능·CPU효과. Script API 호출 수가 많다는 사실은 병목 확정이 아니다. 새로운 네이티브 메모리 수정·보간·업데이트 생략은 없다.

## 5. 검증 수준
빌드·서명/검증·packed바이트·정적변환 증명 완료. CfgConvert 통과는 Enforce 컴파일 통과가 아니다. Enforce 컴파일/런타임/8명/64명/시각/반복성능/최종수용은 미실행이다. 현재 운영 빌드로 채택하지 않는다.

## 6. 변경 사항
격리 nf_contact_timers.pbo SHA BBCAC26DD0A80F578C3789D236DFADDF6CC2108C85AE27E58579F0F746F6BC67. 기본OFF이며 nf-contact-timers-off.flag가 있을 때만 활성화한다. 활성화하면 ContactTicks/Bone/Foot/World/Fallback/Diagnostic의 상세 시간 통계가 갱신되지 않으므로 unavailable 로그를 남긴다. 질의/상태 카운터와 실패 증거 출력은 유지한다. 기존 PBO/서명/자산/서버/키는 변경하지 않았다. 기존 개인키는 서명에 읽기만 사용하고 배포물에 포함하지 않았다. 제거/flag 해제로 복귀한다.

## 7. 원시 증거
source-proof.json,packed-proof.json,build.json/signature.log와 후보 소스/config/build스크립트,8개체계획/mission,validation-contract.json. 기존 계측 boundary/source 및 기준 실행 카운터를 내부 체크포인트 해시로 연결하며 원시 파일 자체는 공개하지 않는다. 정적 검토는 관련 현재 로드 구성에 한정하며 모든 과거 실험의 미실행을 증명하지 않는다.

## 8. 잔여 문제
전용 시험 시간이 확보되면8명두 arm의컴파일/4경유/최종상태/정상낙하복귀/수신기동일성을 확인한다. 후보에서 timing필드0과 실제solves/rays/sweeps 양수를 확인하고, 진단 시간0을 성능0으로 보고하지 않는다. 기능 통과 후64개체3쌍에서 외부process CPU·Present·game/native진행률을 비교한다. 순서반전·각쌍CPU감소·중앙5%·Present악화5%이내를 사전 고정했다. 실패하면 원본으로 복귀하고 다른 물리 최적화나 기능 개발로 확대하지 않는다. 성능 효과가 작거나 없으면 이 후보를 해법으로 포장하지 않는다.
