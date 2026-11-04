# 5파트14클립 입력 준비와 소비 계약 조사

## 1. 작업 목표
정지Run3쌍에서 관찰된 5파트의 반복 이득을 실제 후보로 옮기기 전에14클립 자산 연결과 네트워크 생성 계약을 조사한다. 기존8파트 빌드·게임플레이·PBO는 보존한다.

## 2. 실제 수행
기존70개(5파트×14클립) 정적 키 계약을 재사용해 새65개 TXA를 생성했다. 이미 검증된5개 골격·5개Run·5개 모델은 재사용한다. 부모/키 토큰·60fps·프레임 수를 대조하고5개 ASI가 각14개 클립을 참조하도록 별도 바인딩 소스를 만들었다. 원래 native_traversal 그래프를 재사용하며 상태·명령을 새로 설계하지 않았다. 네트워크 Initialize의8파트 상수·고정 타입·충돌차단·피해수신 개체 수명을 읽기 전용으로 추적했다. 컴파일 감사 도구는 새 출력이 없으면 통과하지 않도록 준비했다.

## 3. 측정 결과
새TXA65개, 재사용Run5개, 전체70파트/클립 계약. 신규 입력+변환플러그인+격리프로젝트44,874,485bytes. 부모/유지 키/프레임헤더 비교 통과. 원래 모델·골격 추가복사0. 컴파일 사전 감사: 기존검증Run5개만 존재, 신규컴파일0/65, 네이티브 전체검증 미통과(미실행). 새 DayZ 실행0, Workbench 시작2회(첫 숨김 창은 조작 대상으로 나타나지 않아 소유 프로세스만 강제종료; 두 번째 표시 창은 보안 승인 대기). p50/p95/p99·기능·시각·성능 신규 측정 없음. 이전3쌍의 성능 수치를 새로운 실행 결과로 갱신하지 않는다.

## 4. 원인 분석
확인: 실제 네트워크 생성 경로가 RenderPartCount!=8을 거부하고 타입 이름도 고정한다. 게임플레이 경로는 파트별 sibling dBlock 및 별도 DamageReceiver와 충돌차단을 생성한다.8파트의형제쌍28개,5파트10개는 루프에서 계산한 개수이지 비용 절감 측정값이 아니다. 정지Run 시험 생성 코드에는 이 전체 구성의 충돌차단·피해수신 개체가 없으므로 반복 이득을 통합 후 절감률로 직접 옮길 수 없다. 고정8파트 조건만 지우면 이 수명 계약을 누락할 수 있어 아직 수정하지 않았다. 컴파일이 진행되지 않은 직접 원인은 Workbench 내장 pluginfilehandler.c:61의 OpenFile 사용을 묻는 Script Authorization Required 창이다. 승인 우회·자동승인·보안설정 변경은 하지 않았다. 자산 오류로 판정하지 않는다.

## 5. 검증 수준
정적 입력·바인딩/소비 코드 조사: 진단 성공. 신규 네이티브 컴파일·14클립 CPU포즈·루트·이벤트·GPU skin·8/64 기능·성능: 미실행. 준비 완료와 컴파일 완료를 구분한다. 원래20ms 목표 및 전체골 완료 아님. 이 보고서는 종료된 정적 준비 묶음이며 아직 시작하지 못한 네이티브 변환 실험의 완료 보고서가 아니다.

## 6. 변경 사항
새 격리 palette64-fullclips-inputs1과 stage_palette64_fullclips_20261105.py, stage_palette64_fullclip_bindings_20261105.py, audit_palette64_fullclips_20261105.py만 추가했다. 기존 프로필·원본·원래Workbench프로젝트·게임플레이·PBO 변경 없음. 새 PBO/서명 대상 없음. Workbench 표시는 승인된 애니메이션 편집 작업용이며 미해결 종료 assertion을 이번 작업에서 고치려 하지 않는다.

## 7. 원시 증거
palette64-fullclips-inputs1/staging.json, binding-plan.json, compile-preflight.json, consumer-contract-review.json, launch.json, hidden-start-terminal.json, visible-launch.json. 기존 palette-budget-review-20261104/inmemory-key-contract.json과 palette64-five-run1/plan.json을 연결한다. source·출력 해시를 재검증했고 원시TXA/모델/네이티브자산은 공개하지 않는다. 살아있는 표시 Workbench는 승인창에서 대기 중이며 종료됐다고 보고하지 않는다. native 변환 플러그인은 아직 실행하지 않았다.

## 8. 잔여 문제
사용자가 Workbench 파일접근 승인 내용을 직접 검토·처리해야 다음 컴파일을 진행할 수 있다. 승인 후65개 변환 → FORM/ANIMSET6·meta·fps/frames·소스해시 감사 → 14클립 full250/5subset CPU행렬/루트/시계 대조 순서로 진행한다. 원래8파트 Initialize는 보존하고, 추후 별도flag의 count/type 선택만 검토하며 형제충돌차단·피해수신·퇴역을 유지한다.8→64 기능/비용 검증 전 게임플레이 교체 금지. 물리 질의의 큰 비용은 여전히 미해결이며 이 준비를 성능 개선으로 보고하지 않는다.
