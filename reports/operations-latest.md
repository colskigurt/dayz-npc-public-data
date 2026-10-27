# 최신 작업 완료 기록 — B안 단일 운영 전환

완료 묶음: 2026-10-28, b-only-20261028. 새 게임 실험을 수행한 기록이 아니라 공개 자료 운영 경로 변경이다.

## 1. 작업 목표

사용자 요청에 따라 앞으로 GitHub B안만 사용한다. GPT 대시보드의 공개 화면·이전 API 제공을 종료하고, 의미 있는 작업 완료 때마다 B안 자료를 갱신하고 답변에 최신 링크를 제공한다.

## 2. 실제 수행

기존 Sites 도구에는 Site 프로젝트 삭제 기능이 없어 플랫폼 기록의 완전 삭제는 수행할 수 없었다. 대신 공개 배포물에서 대시보드·JS·정적 데이터 파일을 모두 제거하고, 모든 경로에 HTTP410 운영 종료 응답만 제공하도록 배포했다. 원본 자료·이전 보고서·과거 배포 소스는 로컬/버전 이력에 보존했다. 공개 자료는 B안으로만 안내한다.

B안 README를 단일 진입점으로 바꾸고 AI_REVIEW.md/실험 요약의 활성 링크를 GitHub Raw로 변경했다. 과거 보고서의 옛 사이트 언급은 당시 이력이므로 덮어쓰지 않았다. 실험 JSON·지표·선정 함수/대표 로그의 내용은 변경하지 않았다.

## 3. 측정 결과

종료 Worker의 GET/HEAD 경로별 검사14개 통과. 실제 기존 도메인의 루트/API/보고서/app.js/최소 테스트 JSON/다운로드6개 경로에서 인증 없는 HTTP410을 확인했다. 응답 Content-Type은 text/plain; charset=utf-8, Cache-Control은 no-store, X-Robots-Tag는 noindex, noarchive다. 기존 대시보드 HTML이나 JSON은 반환하지 않았다.

이전 B안 게시 묶음에서는 전체 파일 SHA/JSON 스키마와3개 외부 네트워크의 요약 본문 일치를 확인했다. 이번 링크 변경 뒤의 실제 HTTP/해시는 verification/latest.json의 별도 검증 시각과 결과를 확인한다. 게시 시각을 새 실험 수행 시각으로 해석하지 않는다.

NPC8/64 안정성·프레임 p50/p95/p99·애니메이션 품질은 이번 묶음에서 새로 측정하지 않았다. 기존 실험 결과와20ms p95 미달성 상태는 그대로다.

## 4. 원인 분석

확인: old dashboard는 더 이상 제공되지 않으며 기존 경로는410이다. B안 데이터는 별도 공개 GitHub 저장소에서 운영한다. 플랫폼 삭제 기능 부재와 공개 콘텐츠 철거는 다른 상태다.

미확정: 새 Raw URL의 ChatGPT 웹 도구 Cache miss 원인은 해결되지 않았다. HTTP 접근 성공·게시 성공·특정 AI 도구 읽기를 구분한다. 과거 Site 프로젝트의 서버 측 메타데이터/배포 이력이 완전 삭제됐다고 주장하지 않는다.

## 5. 검증 수준

- 구현·회귀 통과: 종료 응답14개 검사.
- 실배포 진단 성공: 기존 도메인의 실제6개 경로410.
- 미완료: 플랫폼 Site 프로젝트 레코드의 완전 삭제. 현재 도구에 해당 기능이 없다.
- 미수용: 특정 ChatGPT 웹 도구의 새 Raw URL 읽기.
- 이번에 측정하지 않음: 새 NPC 동작·시각 품질·성능 개선.

## 6. 변경 사항

Sites 종료 배포 version7, source af07bda044909bd2109b5053c1fe4909dd354a86. 이전 화면 대신 운영 종료 텍스트와 B안 주소만 반환한다. 향후 기존 Site를 재생성하거나 대시보드로 재배포하지 않는다.

B안의 활성 진입점은 README, 최신 작업 완료 기록은 이 파일, 최신 실험 요약은 reports/latest.md, 단일 AI 검토 문서는 AI_REVIEW.md다. 로컬 AGENTS 지침을 B안 단일 운영·작업 완료 시 갱신·링크 제공으로 변경한다. 과거 원본 실험/보고서/PBO/게임 코드는 변경하지 않았다.

## 7. 원시 증거

실행 ID b-only-20261028. 종료 배포 수신 확인, retired-site-http.json, 링크 변경 내역, B안 manifest와 HTTP 검증 결과를 로컬 체크포인트에서 SHA-256 검증 후 보존한다. 공개 문서에는 로컬 경로·프로세스·자격증명을 넣지 않는다.

- [최신 실험 요약](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/latest.md)
- [AI 단일 검토 문서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/AI_REVIEW.md)
- [상태 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/status.json)
- [검증 기록](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/verification/latest.json)

## 8. 잔여 문제와 다음 작업

향후 의미 있는 작업/실험 묶음이 안전하게 끝나면 다음 순서로 처리한다: 기존 로컬 체크포인트 생성 → 공개 범위/민감 정보/원본 무결성 확인 → B안의 작업 기록·실험 요약·JSON·검토 패킷 갱신 → 커밋·게시 → 실제 Raw GET/스키마/해시 검증 → 완료 답변에 B안 최신 링크와 해당 커밋 링크 제공. 주기적 백그라운드 감시나 새 게임 실험 실행을 뜻하지 않는다.

새 도구, 사용자 피드백과 반영 상태, 발견한 누락/수정점, 다음 실험 우선순위와 성공 기준도 같은 기록에 포함한다. 외부 AI 접근 상태는 실제 재검증 전까지 미수용으로 유지한다. Site 프로젝트 자체의 완전 삭제가 필요하면 플랫폼 관리 UI의 삭제 기능이 별도로 필요하다.
